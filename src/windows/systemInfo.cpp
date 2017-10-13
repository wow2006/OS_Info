#include "windows\systemInfo.hpp"
#include <VersionHelpers.h>
#include <iostream>
#include <array>
#include <vector>
#include <intrin.h>  

typedef BOOL(WINAPI *LPFN_GLPI)(
  PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
  PDWORD);

using map = systemInfo::map;

// Helper function to count set bits in the processor mask.
DWORD systemInfo::CountSetBits(ULONG_PTR bitMask)
{
  DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
  DWORD bitSetCount = 0;
  ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
  DWORD i;

  for (i = 0; i <= LSHIFT; ++i)
  {
    bitSetCount += ((bitMask & bitTest) ? 1 : 0);
    bitTest /= 2;
  }

  return bitSetCount;
}

void systemInfo::readSystemInfo(map& kernelInfo, map& cpuInfo, map& memInfo) {
  {// read https://msdn.microsoft.com/en-us/library/windows/desktop/ms683194(v=vs.85).aspx
   // Read CPU Count cores and Cache
    DWORD byteOffset   = 0;
    DWORD returnLength = 0;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;

    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;

    DWORD processorL1Cache = 0;
    DWORD processorL2Cache = 0;
    DWORD processorL3Cache = 0;

    DWORD processorPackageCount = 0;
    PCACHE_DESCRIPTOR Cache;

    LPFN_GLPI glpi = (LPFN_GLPI)GetProcAddress(
      GetModuleHandle(TEXT("kernel32")),
      "GetLogicalProcessorInformation");

    while (!done)
    {
      DWORD rc = glpi(buffer, &returnLength);

      if (FALSE == rc)
      {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
          if (buffer)
            free(buffer);

          buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
            returnLength);

          if (NULL == buffer) break;
        }
        else
        {
          break;
        }
      }
      else
      {
        done = TRUE;
      }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
      switch (ptr->Relationship)
      {
      case RelationNumaNode:
        // Non-NUMA systems report a single record of this type.
        numaNodeCount++;
        break;

      case RelationProcessorCore:
        processorCoreCount++;

        // A hyperthreaded core supplies more than one logical processor.
        logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
        break;

      case RelationCache:
        // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
        Cache = &ptr->Cache;
        if (Cache->Level == 1)
        {
          processorL1CacheCount++;
          processorL1Cache = Cache->Size;
        }
        else if (Cache->Level == 2)
        {
          processorL2CacheCount++;
          processorL2Cache = Cache->Size;
        }
        else if (Cache->Level == 3)
        {
          processorL3CacheCount++;
          processorL3Cache = Cache->Size;
        }
        break;

      case RelationProcessorPackage:
        // Logical processors share a physical package.
        processorPackageCount++;
        break;

      default:
        break;
      }
      byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
      ptr++;
    }

    cpuInfo["CpuCache"] = std::to_string(int(processorL1Cache / 1024)) + "kB";
    cpuInfo["CpuCache2"] = std::to_string(int(processorL2Cache / 1024)) + "kB";
    cpuInfo["CpuCache3"] = std::to_string(int(processorL3Cache / 1024)) + "kB";
    cpuInfo["CpuCores"] = std::to_string(processorCoreCount);
    cpuInfo["CpuHyperthreadingCount"] = std::to_string(logicalProcessorCount);
  }

  SYSTEM_INFO info;
  GetSystemInfo(&info);
  // read https://msdn.microsoft.com/en-us/library/windows/desktop/ms724958(v=vs.85).aspx
  switch (info.wProcessorArchitecture)
  {
  case PROCESSOR_ARCHITECTURE_AMD64:
    kernelInfo["CPU_Arch"] = "x64";
    break;
  case PROCESSOR_ARCHITECTURE_INTEL:
    kernelInfo["CPU_Arch"] = "x86";
    break;
  case PROCESSOR_ARCHITECTURE_ARM:
    kernelInfo["CPU_Arch"] = "ARM";
    break;
  }

  { // read https://msdn.microsoft.com/en-us/library/hskdteyh.aspx
    std::vector<std::array<int, 4>> data_;
    std::vector<std::array<int, 4>> extdata_;

    std::array<int, 4> cpui;
    __cpuid(cpui.data(), 0);
    auto nIds_ = cpui[0];

    for (int i = 0; i <= nIds_; ++i) {
      __cpuidex(cpui.data(), i, 0);
      data_.push_back(cpui);
    }
    // Capture vendor string  
    char vendor[0x20];
    memset(vendor, 0, sizeof(vendor));
    *reinterpret_cast<int*>(vendor) = data_[0][1];
    *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
    *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
    cpuInfo["CpuVendorId"] = vendor;

    // Calling __cpuid with 0x80000000 as the function_id argument  
    // gets the number of the highest valid extended ID.  
    __cpuid(cpui.data(), 0x80000000);
    auto nExIds_ = cpui[0];

    char brand[0x40];
    memset(brand, 0, sizeof(brand));

    for (int i = 0x80000000; i <= nExIds_; ++i)
    {
      __cpuidex(cpui.data(), i, 0);
      extdata_.push_back(cpui);
    }

    // Interpret CPU brand string if reported  
    if (nExIds_ >= 0x80000004)
    {
      memcpy(brand, extdata_[2].data(), sizeof(cpui));
      memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
      memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
      cpuInfo["CpuModel"] = brand;
    }
  }

  {//
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    memInfo["MemTotal"] = std::to_string((statex.ullTotalPhys / 1024) / 1024) + "MB";

    memInfo["SwapTotal"] = std::to_string((statex.ullTotalPageFile / 1024) / 1024) + "MB";

    memInfo["MemFree"] = std::to_string((statex.ullAvailPhys / 1024) / 1024) + "MB";
  }
}

std::string systemInfo::getWindowsName() {
  if (IsWindows10OrGreater())
  {
    return "Win10";
  }
  if (IsWindows8Point1OrGreater())
  {
    return "Win8.1";
  }
  if (IsWindows8OrGreater())
  {
    return "Win8";
  }
  if (IsWindows7SP1OrGreater())
  {
    return "Win7SP10";
  }
  if (IsWindows7OrGreater())
  {
    return "Win7SP10";
  }

  return "Unknown";
}
