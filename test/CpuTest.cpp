#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE("CPU Model", "[CpuModel]") {
  auto CPU_Name = getCommand("awk -F: '/model name/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' /proc/cpuinfo");
  REQUIRE(SystemInfo::getCPU_Name() == CPU_Name);
}

TEST_CASE("CPU Vendor", "[CpuVendorId]") {
  auto CPU_Vendor = getCommand("awk -F: '/vendor_id/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' /proc/cpuinfo");
  REQUIRE(SystemInfo::getCPU_Vendor() == CPU_Vendor);
}

TEST_CASE("CPU Cache", "[CpuCache]") {
  auto CPU_Cache = getCommand("awk -F: '/cache/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' /proc/cpuinfo");
  REQUIRE(SystemInfo::getCPU_Cache() == CPU_Cache);
}

TEST_CASE("CPU Cores", "[CpuCores]") {
    auto CPU_Cores = getCommand("awk -F: '/cpu cores/ { sub(/^[ ]+/, \"\", $2); print $2; exit }' /proc/cpuinfo");
    REQUIRE(SystemInfo::getCPU_Cores() == CPU_Cores);
}

TEST_CASE("CPU Hyperthreading count", "[CpuHyperthreadingCount]") {
    auto hThreadCount = getCommand("grep -c \"processor\" /proc/cpuinfo");
    REQUIRE(SystemInfo::getCPU_hyperThreadingCount() == hThreadCount);
}
