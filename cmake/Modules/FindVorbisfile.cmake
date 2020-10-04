find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
	pkg_check_modules(PC_VORBISFILE QUIET vorbisfile)
endif()

set(VORBISFILE_DEFINITIONS ${PC_VORBISFILE_CFLAGS_OTHER})

find_path(Vorbisfile_INCLUDE_DIR vorbis/vorbisfile.h
          HINTS ${PC_VORBISFILE_INCLUDEDIR} ${PC_VORBISFILE_INCLUDE_DIRS})

find_library(Vorbisfile_LIBRARY NAMES vorbisfile libvorbisfile
             HINTS ${PC_VORBISFILE_LIBDIR} ${PC_VORBISFILE_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vorbisfile
		REQUIRED_VARS
		Vorbisfile_LIBRARY Vorbisfile_INCLUDE_DIR)

mark_as_advanced(LIBVORBISFILE_LIBRARY LIBVORBISFILE_INCLUDE_DIR)

set(Vorbisfile_LIBRARIES ${Vorbisfile_LIBRARY})
set(Vorbisfile_INCLUDE_DIRS ${Vorbisfile_INCLUDE_DIR})

if (Vorbisfile_FOUND AND NOT TARGET Vorbisfile::Vorbisfile)
	add_library(Vorbisfile::Vorbisfile UNKNOWN IMPORTED)
	set_target_properties(Vorbisfile::Vorbisfile PROPERTIES
		IMPORTED_LOCATION "${Vorbisfile_LIBRARY}"
		INTERFACE_COMPILE_OPTIONS "${PC_VORBISFILE_CFLAGS_OTHER}"
		INTERFACE_INCLUDE_DIRECTORIES "${Vorbisfile_INCLUDE_DIR}"
	)
endif()
