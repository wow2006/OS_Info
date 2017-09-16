#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"

TEST_CASE("CPU Model", "[CpuModel]") {
    REQUIRE(SystemInfo::getCPU_Name() == "Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz");
}

TEST_CASE("CPU Vendor", "[CpuVendorId]") {
    REQUIRE(SystemInfo::getCPU_Vendor() == "GenuineIntel");
}

TEST_CASE("CPU Cache", "[CpuCache]") {
    REQUIRE(SystemInfo::getCPU_Cache() == "6144 KB");
}

TEST_CASE("CPU Cores", "[CpuCores]") {
    REQUIRE(SystemInfo::getCPU_Cores() == "4");
}
