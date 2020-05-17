find_path(LIBGME_INCLUDE_DIR gme/gme.h
	PATHS deps/game-music-emu-0.6.2 )

find_library(LIBGME_LIBRARY NAMES gme libgme)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibGme DEFAULT_MSG
		LIBGME_LIBRARY LIBGME_INCLUDE_DIR)

mark_as_advanced(LIBGME_LIBRARY LIBGME_INCLUDE_DIR)

set(LIBGME_LIBRARIES ${LIBGME_LIBRARY})
set(LIBGME_INCLUDE_DIRS ${LIBGME_INCLUDE_DIRS})
