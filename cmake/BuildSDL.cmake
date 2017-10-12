find_package(OpenGL REQUIRED)

if(UNIX)
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
	find_package(SDL2 REQUIRED)
	if(NOT SDL2_FOUND)
		message("Install SDL from package manager")
	endif()
else()
	set(TAR_FILE "${CMAKE_SOURCE_DIR}/3rdparty/SDL2-devel-2.0.6-VC.zip")
	set(TEMP_DIR "${CMAKE_SOURCE_DIR}/3rdparty/SDL2-2.0.6")
	set(TARGET_DIR "${CMAKE_SOURCE_DIR}/3rdparty/SDL2")
	if(EXISTS ${TAR_FILE})
		add_custom_target(SDL2)
	else()
		include(ExternalProject)
		ExternalProject_Add(
		SDL2
		URL https://www.libsdl.org/release/SDL2-devel-2.0.6-VC.zip
		DOWNLOAD_DIR    "${CMAKE_SOURCE_DIR}/3rdparty/"
		SOURCE_DIR      ${TEMP_DIR}
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		TEST_COMMAND "")
	endif()
	
	add_custom_command(TARGET SDL2 POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
	"${TEMP_DIR}/include/"
	"${TARGET_DIR}/include/SDL2")
	add_custom_command(TARGET SDL2 POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
	"${TEMP_DIR}/lib/"
	"${TARGET_DIR}/lib")

	add_custom_command(TARGET SDL2 POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E remove_directory "${TEMP_DIR}")
	add_custom_command(TARGET SDL2 POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy
	"${TARGET_DIR}/lib/x64/SDL2.dll" "${CMAKE_BINARY_DIR}/SDL2.dll")

	set(SDL2_INCLUDE_DIR "${TARGET_DIR}/include")
	set(SDL2_LIBRARY "${TARGET_DIR}/lib/x64/SDL2.lib"
					 "${TARGET_DIR}/lib/x64/SDL2main.lib")
endif()

#if(NOT SDL2_FOUND)
#	message("Building SDL from source")
#	option(SDL_ATOMIC     "" OFF)
#	option(SDL_AUDIO      "" OFF)
#	option(SDL_DLOPEN     "" OFF)
#	option(SDL_EVENTS     "" OFF)
#	option(SDL_FILE       "" OFF)
#	option(SDL_FILESYSTEM "" OFF)
#	option(SDL_HAPTIC     "" OFF)
#	option(SDL_JOYSTICK   "" OFF)
#	#option(SDL_LOADSO    "" OFF)
#	option(SDL_POWER      "" OFF)
#	option(SDL_SHARED     "" OFF)
#	option(RENDER_D3D     "" OFF)
#	option(VIDEO_DUMMY    "" OFF)
#	option(VIDEO_OPENGLES "" OFF)
#	option(VIDEO_VULKAN   "" OFF)
#	option(DIRECTX        "" OFF)
#	add_subdirectory("${CMAKE_SOURCE_DIR}/3rdparty/SDL")
#endif()