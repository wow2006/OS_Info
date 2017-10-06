find_package(SDL2 REQUIRED)

if(UNIX)
  set(src DebugInfoLinux.cpp linux/kernelInfo.cpp
      linux/hardwareInfo.cpp linux/sdlInfo.cpp
      linux/glibcInfo.cpp)
elseif(WIN32)
  set(src "")
endif()

add_library(SystemInfo STATIC ${src})
target_include_directories(SystemInfo PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include ${SDL2_INCLUDE_DIR})
target_link_libraries(SystemInfo ${SDL2_LIBRARY})
target_compile_features(SystemInfo PUBLIC cxx_auto_type)
