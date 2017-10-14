find_package(OpenGL REQUIRED)

if(UNIX)
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
	find_package(SDL2 REQUIRED)
	if(NOT SDL2_FOUND)
		message("Install SDL from package manager")
	endif()
else()

if(WIN32)
  set(URL https://www.libsdl.org/release/SDL2-devel-2.0.6-VC.zip)
  set(TAR_FILE "${CMAKE_SOURCE_DIR}/3rdparty/SDL2-devel-2.0.6-VC.zip")
else()
  set(URL https://www.libsdl.org/release/SDL2-devel-2.0.6-mingw.tar.gz)
  set(TAR_FILE "${CMAKE_SOURCE_DIR}/3rdparty/SDL2-devel-2.0.6-mingw.tar.gz")
endif()

	set(TEMP_DIR "${CMAKE_SOURCE_DIR}/3rdparty/SDL2-2.0.6")
	set(TARGET_DIR "${CMAKE_SOURCE_DIR}/3rdparty/SDL2")

  if(EXISTS ${TAR_FILE})
		add_custom_target(SDL2)
		add_custom_command(TARGET SDL2 POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E tar -x "${TAR_FILE}"
		WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rdparty/")
	else()
		include(ExternalProject)
		ExternalProject_Add(
		SDL2
		URL ${URL}
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
