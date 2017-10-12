if(UNIX)
  set(src DebugInfoLinux.cpp linux/kernelInfo.cpp
      linux/hardwareInfo.cpp linux/glibcInfo.cpp)
elseif(WIN32)
  set(src DebugInfoWindows.cpp windows/systemInfo.cpp)
endif()

set(include_list ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(link_list "")

message("SDL2_INCLUDE_DIR : ${SDL2_INCLUDE_DIR}")
if(USE_SDL)
	list(APPEND link_list    ${SDL2_LIBRARY})
	list(APPEND include_list ${SDL2_INCLUDE_DIR})
	list(APPEND src linux/sdlInfo.cpp)
endif()

add_library(SystemInfo STATIC ${src})
target_include_directories(SystemInfo PUBLIC ${include_list})
target_link_libraries(SystemInfo ${link_list})

target_compile_features(SystemInfo PUBLIC cxx_auto_type)
