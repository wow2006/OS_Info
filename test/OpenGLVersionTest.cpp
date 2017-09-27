#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DebugInfo.hpp"
#include "testutility.hpp"

TEST_CASE("OpenGL Version", "[OpenGLVersion]") {
  auto OpenGL_Version = getCommand("glxinfo -B | awk -F: '/OpenGL version string/{ sub(/^[ ]+/, \"\", $2); print $2 }'");
  REQUIRE(SystemInfo::getOpenGL_Version() == OpenGL_Version);
}
