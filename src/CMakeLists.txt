if(UNIX)
  set(src linux/kernelInfo.cpp
      linux/hardwareInfo.cpp linux/glibcInfo.cpp)
elseif(WIN32)
  set(src windows/systemInfo.cpp)
endif()

set(include_list ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(link_list "")

if(USE_SDL)
	list(APPEND link_list    ${SDL2_LIBRARY})
	list(APPEND include_list ${SDL2_INCLUDE_DIR})
	list(APPEND src sdlInfo.cpp)
endif()

add_library(SystemInfo STATIC DebugInfo.cpp ${src})
target_include_directories(SystemInfo PUBLIC ${include_list})
target_link_libraries(SystemInfo "-lmingw32 -mwindows -mconsole -LC:/Users/Test/Source/Repos/OS_Info/3rdparty/SDL2-2.0.6_MINGW/i686-w64-mingw32/lib -lSDL2main -lSDL2")

target_compile_features(SystemInfo PUBLIC cxx_auto_type)
if(USE_SDL)
  add_dependencies(SystemInfo SDL2)
endif()