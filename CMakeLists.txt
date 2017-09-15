cmake_minimum_required(VERSION 3.5)
project(SystemInfo)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-fcolor-diagnostics)
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
