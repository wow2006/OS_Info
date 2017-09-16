#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string readCmd(const char* _cmd) {
  std::array<char, 128> buffer;
  std::string result;
  FILE* in;
  if(!(in = popen(_cmd, "r"))) {
    std::exit(1);
  }
  while(fgets(buffer.data(), buffer.size(), in) != nullptr) {
    result += buffer.data();
  }
  pclose(in);

  return result;
}

TEST_CASE( "Distro Name", "[DistroName]" ) {
  REQUIRE(SystemInfo::getOS_Name() == "\"KDE neon LTS\"");
}

TEST_CASE( "Distro Version", "[DistroVersion]" ) {
    REQUIRE(SystemInfo::getOS_Version() == "\"5.8\"");
}
