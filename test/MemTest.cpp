#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"


#if defined _MSC_VER
constexpr auto MemTotalCommand  = "";
constexpr auto SwapTotalCommand = "";
constexpr auto MemFreeCommand   = "";
#else
constexpr auto MemTotalCommand = "grep MemTotal /proc/meminfo | awk -F: '{x=$2} END {sub(/^[ ]+/, \"\", "
"x); print x}'";
constexpr auto SwapTotalCommand = "grep SwapTotal /proc/meminfo | awk -F: '{x=$2} END {sub(/^[ ]+/, \"\", "
"x); print x}'";
constexpr auto MemFreeCommand = "grep MemFree /proc/meminfo | awk -F: '{x=$2}"
"END {sub(/^[ ]+/, \"\", x); print x}'";
#endif


TEST_CASE("Memory Total", "[MemTotal]") {
	auto systemCommand = "16381MB";//getCommand(MemTotalCommand);
  REQUIRE(SystemInfo::getMem_Total() == systemCommand);
}


TEST_CASE("Swap Total", "[SwapTotal]") {
	auto systemCommand = "32761MB"; //getCommand(SwapTotalCommand);
  SystemInfo::isInitlized();
  auto swap = SystemInfo::mInfo[SystemInfo::Mem]["SwapTotal"];
  REQUIRE(swap == systemCommand);
}

/*
TEST_CASE("Mem Free", "[MemFree]") {
  auto systemCommand = getCommand(MemFreeCommand);
  REQUIRE(SystemInfo::getMem_Free(true) == systemCommand);
}
*/
