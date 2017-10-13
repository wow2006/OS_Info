#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE("Kernel Name", "[KernelName]") {
  auto kernelName = getCommand("uname");
  REQUIRE(DebugInfo::getKernel_Name() == kernelName);
}

TEST_CASE("CPU Arch", "[CPU_Arch]") {
  auto arch = getCommand("uname -p");
  REQUIRE(DebugInfo::getKernel_Arch() == arch);
}

TEST_CASE("Kernel Release", "[KernelRelease]") {
  auto release = getCommand("uname -r");
  REQUIRE(DebugInfo::getKernel_Release() == release);
}
