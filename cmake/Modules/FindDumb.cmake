find_package(PkgConfig)
pkg_check_modules(PC_DUMB QUIET dumb)
set(DUMB_DEFINITIONS ${PC_DUMB_CFLAGS_OTHER})

find_path(Dumb_INCLUDE_DIR dumb.h
          HINTS ${PC_DUMB_INCLUDEDIR} ${PC_DUMB_INCLUDE_DIRS}
          PATHS deps/dumb-2.0.3/include )

find_library(Dumb_LIBRARY NAMES libdumb.a dumb
             HINTS ${PC_DUMB_LIBDIR} ${PC_DUMB_LIBRARY_DIRS}
             PATHS deps/dumb-2.0.3/ )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Dumb
		REQUIRED_VARS
		Dumb_LIBRARY Dumb_INCLUDE_DIR)

mark_as_advanced(Dumb_LIBRARY Dumb_INCLUDE_DIR)

set(Dumb_LIBRARIES ${Dumb_LIBRARY})
set(Dumb_INCLUDE_DIRS ${Dumb_INCLUDE_DIR})

if (Dumb_FOUND AND NOT TARGET Dumb::Dumb)
	add_library(Dumb::Dumb UNKNOWN IMPORTED)
	set_target_properties(Dumb::Dumb PROPERTIES
		IMPORTED_LOCATION "${Dumb_LIBRARY}"
		INTERFACE_COMPILE_OPTIONS "${PC_DUMB_CFLAGS_OTHER}"
		INTERFACE_INCLUDE_DIRECTORIES "${Dumb_INCLUDE_DIR}"
	)
endif()
