#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

#if defined _MSC_VER
constexpr auto CpuModelCommand = "";
constexpr auto CpuVendorIdCommand = "";
constexpr auto CpuCacheCommand = "";
constexpr auto CpuCoresCommand = "";
constexpr auto CpuHyperthreadingCountCommand = "";
#else
constexpr auto CpuModelCommand = "awk -F: '/model name/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuVendorIdCommand = "awk -F: '/vendor_id/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuCacheCommand = "awk -F: '/cache/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuCoresCommand = "awk -F: '/cpu cores/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' "
"/proc/cpuinfo";
constexpr auto CpuHyperthreadingCountCommand = "grep -c \"processor\" /proc/cpuinfo";
#endif

TEST_CASE("CPU Model", "[CpuModel]") {
  auto CPU_Name = "Intel(R) Core(TM) i7 CPU         920  @ 2.67GHz";// getCommand(CpuModelCommand);
  REQUIRE(SystemInfo::getCPU_Name() == CPU_Name);
}

TEST_CASE("CPU Vendor", "[CpuVendorId]") {
  auto CPU_Vendor = "GenuineIntel";// getCommand(CpuVendorIdCommand);
  REQUIRE(SystemInfo::getCPU_Vendor() == CPU_Vendor);
}

TEST_CASE("CPU Cache", "[CpuCache]") {
  auto CPU_Cache = "32kB";// getCommand(CpuCacheCommand);
  REQUIRE(SystemInfo::getCPU_Cache() == CPU_Cache);
}

TEST_CASE("CPU Cores", "[CpuCores]") {
  auto CPU_Cores = "4";// getCommand(CpuCoresCommand);
  REQUIRE(SystemInfo::getCPU_Cores() == CPU_Cores);
}

TEST_CASE("CPU Hyperthreading count", "[CpuHyperthreadingCount]") {
  auto hThreadCount = "8";// getCommand(CpuHyperthreadingCountCommand);
  REQUIRE(SystemInfo::getCPU_hyperThreadingCount() == hThreadCount);
}
