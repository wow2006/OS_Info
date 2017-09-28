#include "DebugInfo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/utsname.h>
#include <gnu/libc-version.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <GL/gl.h>

std::array<SystemInfo::stringMap, 9> SystemInfo::mInfo;

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
      int hyperThreads = 0;
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
          else if(line.find("processor") != std::string::npos) {
              hyperThreads++;
          }
      }
      info["hyperThreadsCount"] = std::to_string(hyperThreads);
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
bool read_glibc(map& info) {
  info["version"] = gnu_get_libc_version();
  return !info["version"].empty();
}
bool read_SDL(map& sdlVersion, map& displayServer,
              map& glVersionInfo, map& gpu) {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cerr << "Can not Create SDL init\n";
      return false;
  }

  SDL_SysWMinfo sysinfo;
  SDL_Window* window = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
  auto maincontext = SDL_GL_CreateContext(window);
  if(!window)
    return false;

  SDL_GetVersion(&sysinfo.version);

  std::string version = std::to_string(sysinfo.version.major) + "." +
                        std::to_string(sysinfo.version.minor);
  sdlVersion["version"].swap(version);

  SDL_GetWindowWMInfo(window, &sysinfo);
  switch(sysinfo.subsystem) {
    case SDL_SYSWM_UNKNOWN:   break;
    case SDL_SYSWM_WINDOWS:   displayServer["subsystem"] = "Microsoft Windows(TM)";  break;
    case SDL_SYSWM_X11:       displayServer["subsystem"] = "X Window System";        break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
    case SDL_SYSWM_WINRT:     displayServer["subsystem"] = "WinRT";                  break;
#endif
    case SDL_SYSWM_DIRECTFB:  displayServer["subsystem"] = "DirectFB";               break;
    case SDL_SYSWM_COCOA:     displayServer["subsystem"] = "Apple OS X";             break;
    case SDL_SYSWM_UIKIT:     displayServer["subsystem"] = "UIKit";                  break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
    case SDL_SYSWM_WAYLAND:   displayServer["subsystem"] = "Wayland";                break;
    case SDL_SYSWM_MIR:       displayServer["subsystem"] = "Mir";                    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
    case SDL_SYSWM_ANDROID:   displayServer["subsystem"] = "Android";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
    case SDL_SYSWM_VIVANTE:   info["subsystem"] = "Vivante";                break;
#endif
  }

  std::string glVendor = reinterpret_cast<char const *>(glGetString(GL_VENDOR));
  gpu["glVendor"].swap(glVendor);
  std::string glVersion = reinterpret_cast<char const *>(glGetString(GL_VERSION));
  glVersionInfo["glVersion"].swap(glVersion);

  std::string glRenderer = reinterpret_cast<char const *>(glGetString(GL_RENDERER));
  if(glRenderer.find("NVIDIA") != std::string::npos) {
  const GLenum GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX   = 0x9048;
  const GLenum GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX = 0x9049;
    GLint nTotalMemoryInKB = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
                           &nTotalMemoryInKB);
    std::string TotalMemory = std::to_string(nTotalMemoryInKB) + "KB";
    glVersionInfo["TotalMemory"]. swap(TotalMemory);

    GLint nCurAvailMemoryInKB = 0;
    glGetIntegerv( GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
                           &nCurAvailMemoryInKB );
    std::string currentAvailableMemory = std::to_string(nCurAvailMemoryInKB) + "KB";
    glVersionInfo["CurrentMemory"]. swap(currentAvailableMemory);
  } else if (glRenderer.find("AMD") || glRenderer.find("ATI")) {

  } else if(glRenderer.find("Intel")) {

  }

  gpu["glRenderer"].swap(glRenderer);

  SDL_GL_DeleteContext(maincontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return true;
}

bool SystemInfo::isInitlized() {
    // Read Kernel Info
    auto& kernelInfo = mInfo[SystemInfoKey::Kernel];
    read_uname(kernelInfo);
    // Read CPU Info
    auto& cpuInfo = mInfo[SystemInfoKey::CPU];
    read_cpuInfo(cpuInfo);
    // Read Mem Info
    auto& memInfo = mInfo[SystemInfoKey::Mem];
    read_memInfo(memInfo);
    // Read Distro Info
    auto& distroInfo = mInfo[SystemInfo::Distro];
    if(!read_osRelease(distroInfo)) {
        read_lsbRelease(distroInfo);
    }
    // Read libc Info
    auto& glibc = mInfo[SystemInfoKey::GLibC];
    read_glibc(glibc);
    // Read Display Server
    auto& gpu = mInfo[SystemInfoKey::GPU];
    auto& sdlVersion = mInfo[SystemInfoKey::SDL_Version];
    auto& displayServer = mInfo[SystemInfoKey::DisplayServer];
    auto& glVersion = mInfo[SystemInfoKey::OpenGL];
    read_SDL(sdlVersion, displayServer, glVersion, gpu);

    return true;
}

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
  if(forceToReload){
    read_memInfo(mem);
  } else {
    isInitlized();
  }

  return mem["MemFree"];
}

std::string SystemInfo::getCPU_Name() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if(!isInitlized())
      return "Unknown";
  return cpu["modelName"];
}

std::string SystemInfo::getCPU_Vendor() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if(!isInitlized())
      return "Unknown";
  return cpu["vendorId"];
}

std::string SystemInfo::getCPU_Cache() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if(!isInitlized())
      return "Unknown";
  return cpu["cacheSize"];
}

std::string SystemInfo::getCPU_Cores() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if(!isInitlized())
      return "Unknown";
  return cpu["cpuCores"];
}

std::string SystemInfo::getCPU_hyperThreadingCount() {
  auto& cpu = mInfo[SystemInfoKey::CPU];
  if(!isInitlized())
      return "Unknown";
  return cpu["hyperThreadsCount"];
}

std::string SystemInfo::getOS_Name() {
    if(!isInitlized())
        return "Unknown";
    return mInfo[SystemInfoKey::Distro]["NAME"];
}

std::string SystemInfo::getOS_Version() {
    if(!isInitlized())
        return "Unknown";
    return mInfo[SystemInfoKey::Distro]["VERSION"];
}

std::string SystemInfo::getLibC_Version() {
  auto& glib = mInfo[SystemInfoKey::GLibC];
  if(!isInitlized())
    return "Unknow";
  return glib["version"];
}

std::string SystemInfo::getDisplayServer() {
  auto& displayServer = mInfo[SystemInfoKey::DisplayServer];
  if(!isInitlized())
    return "Unknow";
  return displayServer["subsystem"];
}

std::string SystemInfo::getSDL_Version() {
  auto& SDL = mInfo[SystemInfoKey::SDL_Version];
  if(!isInitlized())
    return "Unknow";
  return SDL["version"];
}

std::string SystemInfo::getGPU_Name() {
  auto& opengl = mInfo[SystemInfoKey::GPU];
  if(!isInitlized())
    return "Unknow";
  return opengl["glRenderer"];
}

std::string SystemInfo::getGPU_Vendor() {
  auto& opengl = mInfo[SystemInfoKey::GPU];
  if(!isInitlized())
    return "Unknow";
  return opengl["glVendor"];
}

std::string SystemInfo::getGPU_Size() {
  auto& opengl = mInfo[SystemInfoKey::GPU];
  if(!isInitlized())
    return "Unknow";
  return opengl["size"];
}

std::string SystemInfo::getOpenGL_Version() {
  auto& opengl = mInfo[SystemInfoKey::OpenGL];
  if(!isInitlized())
    return "Unknow";
  return opengl["glVersion"];
}
