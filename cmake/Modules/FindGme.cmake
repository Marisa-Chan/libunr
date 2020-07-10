find_path(Gme_INCLUDE_DIR gme/gme.h
	PATHS deps/game-music-emu-0.6.2 )

find_library(Gme_LIBRARY NAMES gme libgme)

message("Gme_FOUND: ${Gme_FOUND}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gme
		REQUIRED_VARS
		Gme_LIBRARY Gme_INCLUDE_DIR)

mark_as_advanced(Gme_LIBRARY Gme_INCLUDE_DIR)

set(Gme_LIBRARIES ${Gme_LIBRARY})
set(Gme_INCLUDE_DIRS ${Gme_INCLUDE_DIRS})

if (Gme_FOUND AND NOT TARGET Gme::Gme)
	add_library(Gme::Gme UNKNOWN IMPORTED)
	set_target_properties(Gme::Gme PROPERTIES
		IMPORTED_LOCATION "${Gme_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${Gme_INCLUDE_DIR}"
	)
endif()
