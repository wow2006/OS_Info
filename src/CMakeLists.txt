add_library(SystemInfo STATIC DebugInfoLinux.cpp)
target_include_directories(SystemInfo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_compile_features(SystemInfo PUBLIC cxx_auto_type)
