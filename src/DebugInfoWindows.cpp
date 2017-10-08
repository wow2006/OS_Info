#include "DebugInfo.hpp"
#include "windows\systemInfo.hpp"

std::array<SystemInfo::stringMap, SystemInfo::COUNT> SystemInfo::mInfo;

using map = std::unordered_map<std::string, std::string>;

bool SystemInfo::isInitlized() {
	auto& kernel = mInfo[SystemInfo::Kernel];
	auto& cpu    = mInfo[SystemInfo::CPU];
	auto& distro = mInfo[SystemInfo::Distro];
	auto& mem    = mInfo[SystemInfo::Mem];
	systemInfo::readSystemInfo(kernel, cpu, mem);

	distro["DistroName"]    = "Windows";
	distro["DistroVersion"] = systemInfo::getWindowsName();
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

std::string SystemInfo::getCPU_Name() {
  if (!isInitlized())
    return "Unknown";

  auto& cpuInfo = mInfo[SystemInfoKey::CPU];

  return cpuInfo["CpuModel"];
}

std::string SystemInfo::getCPU_Vendor() {
  if (!isInitlized())
    return "Unknown";

  auto& cpuInfo = mInfo[SystemInfoKey::CPU];

  return cpuInfo["CpuVendorId"];
}

std::string SystemInfo::getCPU_Cache() {
  if (!isInitlized())
    return "Unknown";

  auto& cpuInfo = mInfo[SystemInfoKey::CPU];
  return cpuInfo["CpuCache"];
}

std::string SystemInfo::getCPU_Cores() {
  if (!isInitlized())
    return "Unknown";

  auto& cpuInfo = mInfo[SystemInfoKey::CPU];
  return cpuInfo["CpuCores"];
}

std::string SystemInfo::getCPU_hyperThreadingCount() {
  if (!isInitlized())
    return "Unknown";

  auto& cpuInfo = mInfo[SystemInfoKey::CPU];
  return cpuInfo["CpuHyperthreadingCount"];
}