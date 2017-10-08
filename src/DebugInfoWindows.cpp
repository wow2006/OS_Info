#include "DebugInfo.hpp"
#include <Windows.h>
#include <VersionHelpers.h>
#include <iostream>
#include <intrin.h>  

std::array<SystemInfo::stringMap, SystemInfo::COUNT> SystemInfo::mInfo;

using map = std::unordered_map<std::string, std::string>;

void readSystemInfo(map& kernelInfo, map& cpuInfo, map& memInfo) {
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
	// read cores count
	cpuInfo["cpuCores"] = std::to_string(info.dwNumberOfProcessors);


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
		std::cout << "Total System Memory: " << (statex.ullTotalPhys / 1024) / 1024 << "MB" << std::endl;

		memInfo["MemTotal"] = std::to_string((statex.ullTotalPhys / 1024) / 1024) + "MB";

		memInfo["SwapTotal"] = std::to_string((statex.ullTotalPageFile / 1024) / 1024) + "MB";

		memInfo["MemFree"] = std::to_string((statex.ullAvailPhys / 1024) / 1024) + "MB";
	}
}

std::string getWindowsName() {
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

bool SystemInfo::isInitlized() {
	auto& kernel = mInfo[SystemInfo::Kernel];
	auto& cpu    = mInfo[SystemInfo::CPU];
	auto& distro = mInfo[SystemInfo::Distro];
	auto& mem    = mInfo[SystemInfo::Mem];
	readSystemInfo(kernel, cpu, mem);

	distro["DistroName"]    = "Windows";
	distro["DistroVersion"] = getWindowsName();
	return true;
}

std::string SystemInfo::getOS_Name() {
	if(!isInitlized())
		return "Unknown";

	auto& version = mInfo[SystemInfoKey::Distro];
	auto itr = version.find("DistroName");
	return itr->second;
}

std::string SystemInfo::getOS_Version() {
	if (!isInitlized())
		return "Unknown";

	auto& version = mInfo[SystemInfoKey::Distro];
	auto itr = version.find("DistroVersion");
	return itr->second;
}

std::string SystemInfo::getMem_Total() {
	if (!isInitlized())
		return "Unknown";

	auto& mem = mInfo[SystemInfoKey::Mem];
	return mem["MemTotal"];
}

std::string SystemInfo::getMem_Free(bool) {
	if (!isInitlized())
		return "Unknown";

	auto& mem = mInfo[SystemInfoKey::Mem];
	return mem["MemFree"];
}