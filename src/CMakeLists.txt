find_package(SDL2 REQUIRED)

add_library(SystemInfo STATIC DebugInfoLinux.cpp)
target_include_directories(SystemInfo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${SDL2_INCLUDE_DIR})
target_link_libraries(SystemInfo ${SDL2_LIBRARY})
target_compile_features(SystemInfo PUBLIC cxx_auto_type)
