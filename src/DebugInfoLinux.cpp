#include "DebugInfo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/utsname.h>
#include <gnu/libc-version.h>

/**
Going from the lower level up, we want:
- [x] Kernel and CPU architecture info, that's uname()
- [x] CPU model name, /proc/cpuinfo "model name" line
- [x] Memory Ram, /proc/meminfo
- [x] Distro info, that lsb-release or os-release
- [x] Possibly libc version, for glibc check preprocessor variable __GNU_LIBRARY__ and use gnu_get_glibc_version
- [ ] Display server, use SDL_GetWindowWMInfo
- [ ] Windowing/input toolkit version, use SDL_GetVersion
- [ ] GL version and driver vendor, use OpenGL version call info*
 */

std::unordered_map<std::string,
    std::unordered_map<std::string, std::string>> SystemInfo::mInfo
{ {"Kernel", {}}, {"Distro", {}}, {"CPU", {}}, {"Mem", {}} };

using map = std::unordered_map<std::string, std::string>;

bool read_uname(map& info) {
    if(!info.empty()) return true;
    utsname kernelInfo;

    if(uname(&kernelInfo) == -1)
        return false;

    info["sysname"]  = kernelInfo.sysname;
    info["nodename"] = kernelInfo.nodename;
    info["release"]  = kernelInfo.release;
    info["version"]  = kernelInfo.version;
    info["machine"]  = kernelInfo.machine;
    return true;
}
bool read_cpuInfo(map& info) {
  if(!info.empty()) return true;
  std::ifstream cpuInfo("/proc/cpuinfo");

  if(cpuInfo.is_open()) {
      std::string line;
      while(std::getline(cpuInfo, line)) {
          if(line.find("model name") != std::string::npos) {
              auto pos = line.find(":");
              info["modelName"] = line.substr(pos+2, line.size());
          }
          else if(line.find("vendor_id") != std::string::npos) {
            auto pos = line.find(":");
            info["vendorId"] = line.substr(pos+2, line.size());
          }
          else if(line.find("cache size") != std::string::npos) {
              auto pos = line.find(":");
              info["cacheSize"] = line.substr(pos+2, line.size());
          }
          else if(line.find("cpu cores") != std::string::npos) {
            auto pos = line.find(":");
            info["cpuCores"] = line.substr(pos+2, line.size());
          }
      }
      return true;
  }
  return false;
}
bool read_memInfo(map& info) {
  if(!info.empty()) return true;

  std::ifstream memFile("/proc/meminfo");
  if(memFile.is_open()){
    std::string line;
    while(std::getline(memFile, line)) {
      if(line.find("MemTotal") != std::string::npos) {
          auto pos = line.find(":") + 1;
          while(line[pos] == ' ') pos++;
          info["MemTotal"] = line.substr(pos, line.size());
      }
      if(line.find("SwapTotal") != std::string::npos) {
          auto pos = line.find(":") + 1;
          while(line[pos] == ' ') pos++;
          info["SwapTotal"] = line.substr(pos, line.size());
      }
      if(line.find("MemFree") != std::string::npos) {
          auto pos = line.find(":") + 1;
          while(line[pos] == ' ') pos++;
          info["MemFree"] = line.substr(pos, line.size());
      }
    }
    return true;
  }
  return false;
}
bool read_osRelease(map& info) {
  if(!info.empty()) return true;
  std::ifstream iFile("/etc/os-release");
  if(iFile.is_open()) {
      std::string line;
      while(std::getline(iFile, line)) {
          auto pos = line.find("=");
          if(pos != std::string::npos) {
              info[line.substr(0, pos)] = line.substr(pos+1, line.size());
          }
      }
      return true;
  }
  return false;
}
bool read_lsbRelease(map& info) {
  if(!info.empty()) return true;
  std::ifstream iFile("/etc/lsb-release");
  if(iFile.is_open()) {
      std::string line;
      while(std::getline(iFile, line)) {
          auto pos = line.find("=");
          if(pos != std::string::npos) {
              info[line.substr(0, pos)] = line.substr(pos+1, line.size());
          }
      }
      return true;
  }
  return false;
}
bool read_glibc() { return false; }
bool read_displayServer() { return false; }
bool read_windowAndInput() { return false; }
bool read_glInfo() { return false; }

bool SystemInfo::isInitlized() {
    // Read Kernal Info
    auto& kernelInfo = mInfo["Kernel"];
    read_uname(kernelInfo);
    // Read CPU Info
    auto& cpuInfo = mInfo["CPU"];
    read_cpuInfo(cpuInfo);
    // Read Mem Info
    auto& memInfo = mInfo["Mem"];
    read_memInfo(memInfo);
    // Read Distro Info
    auto& distroInfo = mInfo["Distro"];
    if(!read_osRelease(distroInfo)) {
        read_lsbRelease(distroInfo);
    }
    // Read libc Info
    read_glibc();
    // Read Display Server
    read_displayServer();
    // Read Windowing and input
    read_windowAndInput();
    // Read OpenGL
    read_glInfo();

    return true;
}

std::string SystemInfo::getKernel_Name() {
    auto& kernel = mInfo["Kernel"];
    isInitlized();

    return kernel["sysname"];
}

std::string SystemInfo::getKernel_Arch() {
    auto& kernel = mInfo["Kernel"];
    isInitlized();

    return kernel["machine"];
}

std::string SystemInfo::getKernel_Release() {
    auto& kernel = mInfo["Kernel"];
    isInitlized();

    return kernel["release"];
}

std::string SystemInfo::getMem_Total() {
  auto& mem = mInfo["Mem"];
  isInitlized();

  return mem["MemTotal"];
}

std::string SystemInfo::getMem_Free() {
  auto& mem = mInfo["Mem"];
  isInitlized();

  return mem["MemFree"];
}

std::string SystemInfo::getCPU_Name() {
  auto& cpu = mInfo["CPU"];
  if(!isInitlized())
      return "Unknown";
  return cpu["modelName"];
}

std::string SystemInfo::getCPU_Vendor() {
  auto& cpu = mInfo["CPU"];
  if(!isInitlized())
      return "Unknown";
  return cpu["vendorId"];
}

std::string SystemInfo::getCPU_Cache() {
  auto& cpu = mInfo["CPU"];
  if(!isInitlized())
      return "Unknown";
  return cpu["cacheSize"];
}

std::string SystemInfo::getCPU_Cores() {
  auto& cpu = mInfo["CPU"];
  if(!isInitlized())
      return "Unknown";
  return cpu["cpuCores"];
}

std::string SystemInfo::getOS_Name() {
    if(!isInitlized())
        return "Unknown";
    return mInfo["Distro"]["NAME"];
}

std::string SystemInfo::getOS_Version() {
    if(!isInitlized())
        return "Unknown";
    return mInfo["Distro"]["VERSION"];
}

std::string SystemInfo::getLibC_Version() {
  return gnu_get_libc_version();
}
