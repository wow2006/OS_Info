set(CATCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/Catch/include
    CACHE INTERNAL "Path to include folder for Catch")

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

macro(add_custom_test test_file)
    add_executable(${test_file} ${test_file}.cpp)
    target_link_libraries(${test_file} SystemInfo Catch ${OPENGL_LIBRARIES})
    target_include_directories(${test_file} PUBLIC
        ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_LIST_DIR})
	if(USE_SDL)
		target_link_libraries(${test_file} ${SDL2_LIBRARY})
		target_include_directories(${test_file} PUBLIC ${SDL2_INCLUDE_DIR})
	endif()
    add_test(NAME ${test_file} COMMAND ${test_file})
endmacro(add_custom_test)

add_custom_test(DistroTest)
add_custom_test(KernelTest)
add_custom_test(CpuTest)
add_custom_test(MemTest)
add_custom_test(libcTest)
if(USE_SDL)
	add_custom_test(SDL2Test)
	add_custom_test(OpenGLVersionTest)
endif()