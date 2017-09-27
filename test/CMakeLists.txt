set(CATCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/Catch/include
    CACHE INTERNAL "Path to include folder for Catch")

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
macro(add_custom_test test_file)
    add_executable(${test_file} ${test_file}.cpp)
    target_link_libraries(${test_file} SystemInfo Catch
        ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})
    target_include_directories(${test_file} PUBLIC
        ${OPENGL_INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR} ${SDL2_INCLUDE_DIR})
    add_test(NAME ${test_file} COMMAND ${test_file})
endmacro(add_custom_test)

add_custom_test(DistroTest)
add_custom_test(KernelTest)
add_custom_test(CpuTest)
add_custom_test(MemTest)
add_custom_test(libcTest)
add_custom_test(SDL2Test)
add_custom_test(OpenGLVersionTest)
