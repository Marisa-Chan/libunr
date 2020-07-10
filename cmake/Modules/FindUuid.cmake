find_package(PkgConfig)
pkg_check_modules(PC_UUID QUIET uuid)
set(UUID_DEFINITIONS ${PC_UUID_CFLAGS_OTHER})

find_path(Uuid_INCLUDE_DIR uuid.h
          HINTS ${PC_UUID_INCLUDEDIR} ${PC_UUID_INCLUDE_DIRS})

find_library(Uuid_LIBRARY NAMES uuid
             HINTS ${PC_UUID_LIBDIR} ${PC_UUID_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Uuid DEFAULT_MSG
		Uuid_LIBRARY Uuid_INCLUDE_DIR)

mark_as_advanced(Uuid_LIBRARY Uuid_INCLUDE_DIR)

set(Uuid_LIBRARIES ${Uuid_LIBRARY})
set(Uuid_INCLUDE_DIRS ${Uuid_INCLUDE_DIR})

if (Uuid_FOUND AND NOT TARGET Uuid::Uuid)
	add_library(Uuid::Uuid UNKNOWN IMPORTED)
	set_target_properties(Uuid::Uuid PROPERTIES
		IMPORTED_LOCATION "${Uuid_LIBRARY}"
		INTERFACE_COMPILE_OPTIONS "${PC_UUID_CFLAGS_OTHER}"
		INTERFACE_INCLUDE_DIRECTORIES "${Uuid_INCLUDE_DIR}"
	)
endif()
