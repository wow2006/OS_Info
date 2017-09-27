#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"

TEST_CASE("OpenGL Version", "[OpenGLVersion]") {
    REQUIRE(SystemInfo::getOpenGL_Version() == "4.5.0 NVIDIA 384.59");
}
