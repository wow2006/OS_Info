#include "DebugInfo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/utsname.h>

/**
Going from the lower level up, we want:
- [x] Kernel and CPU architecture info, that's uname()
- [ ] CPU model name, /proc/cpuinfo "model name" line
- [ ] Distro info, that lsb-release or os-release
- [ ] Possibly libc version, for glibc check preprocessor variable __GNU_LIBRARY__ and use gnu_get_glibc_version
- [ ] Display server, use SDL_GetWindowWMInfo
- [ ] Windowing/input toolkit version, use SDL_GetVersion
- [ ] GL version and driver vendor, use OpenGL version call info*
 */

std::unordered_map<std::string,
    std::unordered_map<std::string, std::string>> SystemInfo::mInfo
{ {"Kernel", {}}, {"Distro", {}}, {"CPU", {}} };

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
    std::ifstream cpuInfo("/proc/cpuinfo");

    if(cpuInfo.is_open()) {
        std::string line;
        while(std::getline(cpuInfo, line)) {
            if(line.find("model name") != std::string::npos) {
                auto pos = line.find(":");
                info["modelName"] = line.substr(pos+2, line.size());
            }
            else if(line.find("cache size") != std::string::npos) {
                auto pos = line.find(":");
                info["cacheSize"] = line.substr(pos+2, line.size());
            }
        }
        return true;
    }
    return false;
}
bool read_osRelease(map& info) {
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
    static auto& kernel = mInfo["Kernel"];
    isInitlized();

    return kernel["sysname"];
}

std::string SystemInfo::getKernel_Arch() {
    static auto& kernel = mInfo["Kernel"];
    isInitlized();

    return kernel["machine"];
}

std::string SystemInfo::getKernel_Release() {
    static auto& kernel = mInfo["Kernel"];
    isInitlized();

    return kernel["release"];
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

std::string SystemInfo::getCPU_Name() {
    if(!isInitlized())
        return "Unknown";
    return mInfo["CPU"]["modelName"];
}
