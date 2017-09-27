#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE("Display Server", "[Display Server]") {
  REQUIRE(SystemInfo::getDisplayServer() == "X Window System");
}

TEST_CASE("SDL2 Version", "[SDL2Version]") {
  std::cout << SystemInfo::getSDL_Version() << '\n';
  REQUIRE(SystemInfo::getSDL_Version() == "2.0");
}
