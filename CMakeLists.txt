cmake_minimum_required(VERSION 3.2)
project(SystemInfo)

message("Compiler  : ${CMAKE_CXX_COMPILER_ID}")
message("Version   : ${CMAKE_CXX_COMPILER_VERSION}")
message("Platfor   : ${CMAKE_CXX_PLATFORM_ID}")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-fcolor-diagnostics)
    set(CLANG_COMPILER ON)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(GNU_COMPILER ON)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(MSVC_COMPILER ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(USE_SDL "Use SDL2 to get OpenGL and SDL info" ON)
if(USE_SDL)
	message("Download SDL2")
	include(${CMAKE_SOURCE_DIR}/cmake/BuildSDL.cmake)
endif(USE_SDL)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
