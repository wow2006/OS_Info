#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

#if defined _MSC_VER
constexpr auto DistroNameStr = "";
constexpr auto DistroVersion = "";
#else
constexpr auto DistroNameStr = "awk -F= '/NAME/{ print $2; exit }' /etc/os-release";
constexpr auto DistroVersion = "awk -F= '/VERSION_ID/{ print $2; exit }' /etc/os-release";
#endif

TEST_CASE("Distro Name", "[DistroName]") {
	auto system = "Windows";// getCommand(DistroNameStr);
  REQUIRE(SystemInfo::getOS_Name() == system);
}

TEST_CASE("Distro Version", "[DistroVersion]") {
  auto system = "Win7SP10";
  REQUIRE(SystemInfo::getOS_Version() == system);
}