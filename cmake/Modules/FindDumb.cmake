find_package(PkgConfig)
pkg_check_modules(PC_DUMB QUIET dumb)
set(DUMB_DEFINITIONS ${PC_DUMB_CFLAGS_OTHER})

find_path(LIBDUMB_INCLUDE_DIR dumb.h
          HINTS ${PC_DUMB_INCLUDEDIR} ${PC_DUMB_INCLUDE_DIRS}
          PATHS deps/dumb-2.0.3/include )

find_library(LIBDUMB_LIBRARY NAMES libdumb.a dumb
             HINTS ${PC_DUMB_LIBDIR} ${PC_DUMB_LIBRARY_DIRS}
             PATHS deps/dumb-2.0.3/ )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibDumb2 DEFAULT_MSG
		LIBDUMB_LIBRARY LIBDUMB_INCLUDE_DIR)

mark_as_advanced(LIBDUMB_LIBRARY LIBDUMB_INCLUDE_DIR)

set(LIBDUMB_LIBRARIES ${LIBDUMB_LIBRARY})
set(LIBDUMB_INCLUDE_DIRS ${LIBDUMB_INCLUDE_DIR})
