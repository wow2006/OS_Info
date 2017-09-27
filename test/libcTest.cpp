#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE( "LibC Version", "[LibcVersion]" ) {
  std::cout << SystemInfo::getLibC_Version() << '\n';
  auto system = getCommand("ldd --version | awk '/ldd/ {print $NF}'");
  REQUIRE(SystemInfo::getLibC_Version() == system);
}
