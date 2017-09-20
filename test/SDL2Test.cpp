#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE("SDL2 Version", "[SDL2Version]") {
  REQUIRE(SystemInfo::getDisplayServer() == "X Window System");
}
