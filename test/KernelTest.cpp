#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"

TEST_CASE("Kernel Name", "[KernelName]") {
    REQUIRE(SystemInfo::getKernel_Name() == "Linux");
}

TEST_CASE("CPU Arch", "[CPU_Arch]") {
    REQUIRE(SystemInfo::getKernel_Arch() == "x86_64");
}

TEST_CASE("Kernel Release", "[KernelRelease]") {
    REQUIRE(SystemInfo::getKernel_Release() == "4.10.0-33-generic");
}
