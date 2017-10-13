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

std::array<DebugInfo::stringMap, DebugInfo::COUNT> DebugInfo::mInfo;

using map = std::unordered_map<std::string, std::string>;

#ifdef _MSC_VER
bool DebugInfo::isInitlized() {
  auto& kernel = mInfo[DebugInfo::Kernel];
  auto& cpu = mInfo[DebugInfo::CPU];
  auto& distro = mInfo[DebugInfo::Distro];
  auto& mem = mInfo[DebugInfo::Mem];
  systemInfo::readSystemInfo(kernel, cpu, mem);

  auto& clib = mInfo[DebugInfo::GLibC];
  clib["LibcVersion"] = std::to_string(_MSC_VER);

  distro["DistroName"] = "Windows";
  distro["DistroVersion"] = systemInfo::getWindowsName();
  return true;
}
#else
bool DebugInfo::isInitlized() {
  // Read Kernel Info
  auto& kernelInfo = mInfo[DebugInfo::Kernel];
  KernelInfo::read_uname(kernelInfo);
  // Read CPU Info
  auto& cpuInfo = mInfo[DebugInfo::CPU];
  HardwareInfo::read_cpuInfo(cpuInfo);
  // Read Mem Info
  auto& memInfo = mInfo[DebugInfo::Mem];
  HardwareInfo::read_memInfo(memInfo);
  // Read Distro Info
  auto& distroInfo = mInfo[DebugInfo::Distro];
  if (!DistroInfo::read_distroInfo("/etc/os-release", distroInfo)) {
    DistroInfo::read_distroInfo("/etc/lsb-release", distroInfo);
  }
  // Read libc Info
  auto& glibc = mInfo[DebugInfo::GLibC];
  GlibcInfo::read_glibc(glibc);
  // Read Display Server
  auto& gpu = mInfo[DebugInfo::GPU];
  auto& sdlVersion = mInfo[DebugInfo::SDL_Version];
  auto& displayServer = mInfo[DebugInfo::DisplayServer];
  auto& glVersion = mInfo[DebugInfo::OpenGL];
  SdlInfo::read_SDL(sdlVersion, displayServer, glVersion, gpu);

  return true;
}
#endif

std::string DebugInfo::getKernel_Name() {
  auto& kernel = mInfo[DebugInfo::Kernel];
  isInitlized();

  return kernel["sysname"];
}

std::string DebugInfo::getKernel_Arch() {
  auto& kernel = mInfo[DebugInfo::Kernel];
  isInitlized();

  return kernel["machine"];
}

std::string DebugInfo::getKernel_Release() {
  auto& kernel = mInfo[DebugInfo::Kernel];
  isInitlized();

  return kernel["release"];
}

std::string DebugInfo::getMem_Total() {
  auto& mem = mInfo[DebugInfo::Mem];
  isInitlized();

  return mem["MemTotal"];
}


std::string DebugInfo::getMem_Free(bool forceToReload) {
  auto& mem = mInfo[DebugInfo::Mem];
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

std::string DebugInfo::getCPU_Name() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["modelName"];
}

std::string DebugInfo::getCPU_Vendor() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["vendorId"];
}

std::string DebugInfo::getCPU_Cache() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["cacheSize"];
}

std::string DebugInfo::getCPU_Cores() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["cpuCores"];
}

std::string DebugInfo::getCPU_hyperThreadingCount() {
  auto& cpu = mInfo[DebugInfo::CPU];
  if (!isInitlized()) return "Unknown";
  return cpu["hyperThreadsCount"];
}

std::string DebugInfo::getOS_Name() {
  if (!isInitlized()) return "Unknown";

  auto& kernel = mInfo[DebugInfo::Distro];

  map::iterator itr;
  if ((itr = kernel.find("NAME")) != kernel.end()) {
    return itr->second;
  } else if ((itr = kernel.find("DISTRIB_ID")) != kernel.end()) {
    return itr->second;
  }

  return "Unknown";
}

std::string DebugInfo::getOS_Version() {
  if (!isInitlized()) return "Unknown";

  auto& version = mInfo[DebugInfo::Distro];

  map::iterator itr;
  if ((itr = version.find("VERSION_ID")) != version.end()) {
    return itr->second;
  } else if ((itr = version.find("DISTRIB_RELEASE")) != version.end()) {
    return itr->second;
  }
  return "Unknown";
}

std::string DebugInfo::getLibC_Version() {
  auto& glib = mInfo[DebugInfo::GLibC];
  if (!isInitlized()) return "Unknow";
  return glib["version"];
}

std::string DebugInfo::getDisplayServer() {
  auto& displayServer = mInfo[DebugInfo::DisplayServer];
  if (!isInitlized()) return "Unknow";
  return displayServer["subsystem"];
}

std::string DebugInfo::getSDL_Version() {
  auto& SDL = mInfo[DebugInfo::SDL_Version];
  if (!isInitlized()) return "Unknow";
  return SDL["version"];
}

std::string DebugInfo::getGPU_Name() {
  auto& opengl = mInfo[DebugInfo::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["glRenderer"];
}

std::string DebugInfo::getGPU_Vendor() {
  auto& opengl = mInfo[DebugInfo::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["glVendor"];
}

std::string DebugInfo::getGPU_Size() {
  auto& opengl = mInfo[DebugInfo::GPU];
  if (!isInitlized()) return "Unknow";
  return opengl["size"];
}

std::string DebugInfo::getOpenGL_Version() {
  auto& opengl = mInfo[DebugInfo::OpenGL];
  if (!isInitlized()) return "Unknow";
  return opengl["glVersion"];
}
