#ifndef DEBUG_INFO_HPP
#define DEBUG_INFO_HPP
#include <string> 
#include <unordered_map>

class SystemInfo {
public:
static std::string getKernel_Name();

static std::string getKernel_Arch();

static std::string getKernel_Release();

static std::string getCPU_Name();

static std::string getCPU_Vendor();

static std::string getCPU_Cache();

static std::string getCPU_Cores();

static std::string getMem_Total();

static std::string getMem_Free();

static std::string getOS_Name();

static std::string getOS_Version();

static std::string getLibC_Version();

static std::string getGPU_Name();

static std::string getGPU_Vendor();

static std::size_t getGPU_Size();

static bool isInitlized();

static std::unordered_map<std::string,
    std::unordered_map<std::string, std::string>> mInfo;
};

#endif //!DEBUG_INFO_HPP
