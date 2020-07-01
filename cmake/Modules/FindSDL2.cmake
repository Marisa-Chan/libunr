# Use system configuration if it's available
find_package(SDL2 CONFIG)

# This ONLY exists because of Windows...
if (NOT DEFINED SDL2_LIBRARIES)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(VC_LIB_PATH_SUFFIX lib/x64)
	else()
		set(VC_LIB_PATH_SUFFIX lib/x86)
	endif()

	find_library(SDL2_LIBRARY
		NAMES
			libSDL2
			SDL2
		PATH_SUFFIXES
			${VC_LIB_PATH_SUFFIX}
	)

	find_path(SDL2_INCLUDE_DIR SDL.h)

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(SDL2
		REQUIRED_VARS
		SDL2_LIBRARY SDL2_INCLUDE_DIR)

	mark_as_advanced(SDL2_LIBRARY SDL2_INCLUDE_DIR)

	set(SDL2_LIBRARIES ${SDL2_LIBRARY})
	set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
endif()
