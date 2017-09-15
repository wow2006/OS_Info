#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"

TEST_CASE("CPU Model", "[CpuModel]") {
    REQUIRE(SystemInfo::getCPU_Name() == "Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz");
}
