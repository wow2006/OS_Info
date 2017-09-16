#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"


TEST_CASE( "Distro Name", "[DistroName]" ) {
  auto system = getCommand("grep -m1 NAME /etc/os-release | awk -F= '{print $2}'");
  REQUIRE(SystemInfo::getOS_Name() == system);
}

TEST_CASE( "Distro Version", "[DistroVersion]" ) {
  auto system = getCommand("grep -m1 VERSION /etc/os-release | awk -F= '{print $2}'");
  REQUIRE(SystemInfo::getOS_Version() == "\"5.8\"");
}
