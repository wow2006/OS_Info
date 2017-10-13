#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE("Display Server", "[Display Server]") {
  REQUIRE(DebugInfo::getDisplayServer() == "X Window System");
}

TEST_CASE("SDL2 Version", "[SDL2Version]") {
  REQUIRE(DebugInfo::getSDL_Version() == "2.0");
}
