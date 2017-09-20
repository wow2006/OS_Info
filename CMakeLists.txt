cmake_minimum_required(VERSION 3.5)
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

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
