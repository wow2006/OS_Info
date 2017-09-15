//#define CATCH_CONFIG_MAIN
//#include "catch.hpp"

#include <iostream>
#include <cstdlib>
#include "DebugInfo.hpp"

int main() {
    std::cout << SystemInfo::getKernel_Name() << '\n';
    return 0;
}

/*
TEST_CASE( "Distro Name", "[DistroName]" ) {
    REQUIRE(SystemInfo::getOS_Name() == "\"KDE neon LTS\"");
}

TEST_CASE( "Distro Version", "[DistroVersion]" ) {
    REQUIRE(SystemInfo::getOS_Version() == "\"5.8\"");
}
*/
