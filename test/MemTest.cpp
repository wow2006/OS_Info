#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE("Memory Total", "[MemTotal]") {
  auto systemCommand = getCommand(
      "grep MemTotal /proc/meminfo | awk -F: '{x=$2} END {sub(/^[ ]+/, \"\", "
      "x); print x}'");
  REQUIRE(SystemInfo::getMem_Total() == systemCommand);
}

TEST_CASE("Swap Total", "[SwapTotal]") {
  auto systemCommand = getCommand(
      "grep SwapTotal /proc/meminfo | awk -F: '{x=$2} END {sub(/^[ ]+/, \"\", "
      "x); print x}'");
  SystemInfo::isInitlized();
  auto swap = SystemInfo::mInfo[SystemInfo::Mem]["SwapTotal"];
  REQUIRE(swap == systemCommand);
}

/*
TEST_CASE("Mem Free", "[MemFree]") {
  auto systemCommand = getCommand("grep MemFree /proc/meminfo | awk -F: '{x=$2}
END {sub(/^[ ]+/, \"\", x); print x}'");
  REQUIRE(SystemInfo::getMem_Free(true) == systemCommand);
}
*/
