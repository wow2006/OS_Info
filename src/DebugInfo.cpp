#include "DebugInfo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _MSC_VER
#include "windows\systemInfo.hpp"
#else
#include "linux/kernelInfo.hpp"
#include "linux/hardwareInfo.hpp"
#include "linux/distroInfo.cpp"
#include "linux/sdlInfo.hpp"
#include "linux/glibcInfo.hpp"
#endif

std::array<SystemInfo::stringMap, SystemInfo::COUNT> SystemInfo::mInfo;

using map = std::unordered_map<std::string, std::string>;

#ifdef _MSC_VER
bool SystemInfo::isInitlized() {
  auto& kernel = mInfo[SystemInfo::Kernel];
  auto& cpu = mInfo[SystemInfo::CPU];
  auto& distro = mInfo[SystemInfo::Distro];
  auto& mem = mInfo[SystemInfo::Mem];
  systemInfo::readSystemInfo(kernel, cpu, mem);

  auto& clib = mInfo[SystemInfo::GLibC];
  clib["LibcVersion"] = std::to_string(_MSC_VER);

  distro["DistroName"] = "Windows";
  distro["DistroVersion"] = systemInfo::getWindowsName();
  return true;
}
#else
bool SystemInfo::isInitlized() {
  // Read Kernel Info
  auto& kernelInfo = mInfo[SystemInfoKey::Kernel];
  KernelInfo::read_uname(kernelInfo);
  // Read CPU Info
  auto& cpuInfo = mInfo[SystemInfoKey::CPU];
  HardwareInfo::read_cpuInfo(cpuInfo);
  // Read Mem Info
  auto& memInfo = mInfo[SystemInfoKey::Mem];
  HardwareInfo::read_memInfo(memInfo);
  // Read Distro Info
  auto& distroInfo = mInfo[SystemInfo::Distro];
  if (!DistroInfo::read_distroInfo("/etc/os-release", distroInfo)) {
    DistroInfo::read_distroInfo("/etc/lsb-release", distroInfo);
  }
  // Read libc Info
  auto& glibc = mInfo[SystemInfoKey::GLibC];
  GlibcInfo::read_glibc(glibc);
  // Read Display Server
  auto& gpu = mInfo[SystemInfoKey::GPU];
  auto& sdlVersion = mInfo[SystemInfoKey::SDL_Version];
  auto& displayServer = mInfo[SystemInfoKey::DisplayServer];
  auto& glVersion = mInfo[SystemInfoKey::OpenGL];
  SdlInfo::read_SDL(sdlVersion, displayServer, glVersion, gpu);

  return true;
}
#endif

std::string SystemInfo::getKernel_Name() {
  auto& kernel = mInfo[SystemInfoKey::Kernel];
  isInitlized();

  return kernel["sysname"];
}

std::string SystemInfo::getKernel_Arch() {
  auto& kernel = mInfo[SystemInfoKey::Kernel];
  isInitlized();

  return kernel["machine"];
}

std::string SystemInfo::getKernel_Release() {
  auto& kernel = mInfo[SystemInfoKey::Kernel];
  isInitlized();

  return kernel["release"];
}

std::string SystemInfo::getMem_Total() {
  auto& mem = mInfo[SystemInfoKey::Mem];
  isInitlized();

  return mem["MemTotal"];
}


std::string SystemInfo::getMem_Free(bool forceToReload) {
  auto& mem = mInfo[SystemInfoKey::Mem];
#ifdef _MSC_VER
#else
  if (forceToReload) {
    HardwareInfo::read_memInfo(mem);
  } else {
    isInitlized();
  }
#endif
  return mem["MemFree"];
}

std::string SystemInfo::getCPU_Name() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["modelName"];
}

std::string SystemInfo::getCPU_Vendor() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["vendorId"];
}

std::string SystemInfo::getCPU_Cache() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["cacheSize"];
}

std::string SystemInfo::getCPU_Cores() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["cpuCores"];
}

std::string SystemInfo::getCPU_hyperThreadingCount() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["hyperThreadsCount"];
}

std::string SystemInfo::getOS_Name() {
  if (!isInitlized()) return "Unknown";

  auto& kernel = mInfo[SystemInfoKey::Distro];

  map::iterator itr;
  if ((itr = kernel.find("NAME")) != kernel.end()) {
    return itr->second;
  } else if ((itr = kernel.find("DISTRIB_ID")) != kernel.end()) {
    return itr->second;
  }

  return "Unknown";
}

std::string SystemInfo::getOS_Version() {
  if (!isInitlized()) return "Unknown";

  auto& version = mInfo[SystemInfoKey::Distro];

  map::iterator itr;
  if ((itr = version.find("VERSION_ID")) != version.end()) {
    return itr->second;
  } else if ((itr = version.find("DISTRIB_RELEASE")) != version.end()) {
    return itr->second;
  }
  return "Unknown";
}

std::string SystemInfo::getLibC_Version() {
  auto& glib = mInfo[SystemInfoKey::GLibC];
  if (!isInitlized()) return "Unknow";
  return glib["version"];
}

std::string SystemInfo::getDisplayServer() {
  auto& displayServer = mInfo[SystemInfoKey::DisplayServer];
  if (!isInitlized()) return "Unknow";
  return displayServer["subsystem"];
}

std::string SystemInfo::getSDL_Version() {
  auto& SDL = mInfo[SystemInfoKey::SDL_Version];
  if (!isInitlized()) return "Unknow";
  return SDL["version"];
}

std::string SystemInfo::getGPU_Name() {
  auto& opengl = mInfo[SystemInfoKey::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["glRenderer"];
}

std::string SystemInfo::getGPU_Vendor() {
  auto& opengl = mInfo[SystemInfoKey::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["glVendor"];
}

std::string SystemInfo::getGPU_Size() {
  auto& opengl = mInfo[SystemInfoKey::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["size"];
}

std::string SystemInfo::getOpenGL_Version() {
  auto& opengl = mInfo[SystemInfoKey::OpenGL];
  if (!isInitlized()) return "Unknow";
  return opengl["glVersion"];
}
