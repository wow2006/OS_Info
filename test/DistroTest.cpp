#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"


TEST_CASE( "Distro Name", "[DistroName]" ) {
  auto system = getCommand("awk -F= '/NAME/{ print $2; exit }' /etc/os-release");
  REQUIRE(SystemInfo::getOS_Name() == system);
}

TEST_CASE( "Distro Version", "[DistroVersion]" ) {
  auto system = getCommand("wk -F= '/VERSION/{ print $2; exit }' /etc/os-release");
  REQUIRE(SystemInfo::getOS_Version() == "\"5.8\"");
}
