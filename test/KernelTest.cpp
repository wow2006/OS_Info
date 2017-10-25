#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

#ifdef _WIN32
constexpr auto CPU_ArchCommand      = "echo x64";
#else
constexpr auto KernelNameCommand    = "uname";
constexpr auto CPU_ArchCommand      = "uname -p";
constexpr auto KernelReleaseCommand = "uname -r";
#endif

TEST_CASE("CPU Arch", "[CPU_Arch]") {
  auto arch = getCommand(CPU_ArchCommand);
  REQUIRE(DebugInfo::getKernel_Arch() == arch);
}

#ifndef _WIN32
TEST_CASE("Kernel Release", "[KernelRelease]") {
  auto release = getCommand(KernelReleaseCommand);
  REQUIRE(DebugInfo::getKernel_Release() == release);
}

TEST_CASE("Kernel Name", "[KernelName]") {
  auto kernelName = getCommand(KernelNameCommand);
  REQUIRE(DebugInfo::getKernel_Name() == kernelName);
}
#endif
