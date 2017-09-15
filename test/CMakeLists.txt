set(CATCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/Catch/include CACHE INTERNAL "Path to include folder for Catch")

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

add_executable(DistroTest DistroTest.cpp)
target_link_libraries(DistroTest SystemInfo Catch)
add_test(NAME DistroTest COMMAND DistroTest)

add_executable(KernelTest KernelTest.cpp)
target_link_libraries(KernelTest SystemInfo Catch)
add_test(NAME KernelTest COMMAND KernelTest)

add_executable(CpuTest CpuTest.cpp)
target_link_libraries(CpuTest SystemInfo Catch)
add_test(NAME CpuTest COMMAND CpuTest)
