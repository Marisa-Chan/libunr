find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
	pkg_check_modules(PC_OPENMPT QUIET openmpt)
endif()

set(OPENMPT_DEFINITIONS ${PC_OPENMPT_CFLAGS_OTHER})

find_path(OpenMPT_INCLUDE_DIR libopenmpt/libopenmpt_stream_callbacks_buffer.h
          HINTS ${PC_OPENMPT_INCLUDEDIR} ${PC_OPENMPT_INCLUDE_DIRS}
          PATHS deps/libopenmpt-0.5.0/include )

find_library(OpenMPT_LIBRARY NAMES libopenmpt.so openmpt
             HINTS ${PC_OPENMPT_LIBDIR} ${PC_OPENMPT_LIBRARY_DIRS}
             PATHS deps/libopenmpt-0.5.0/ )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenMPT
		REQUIRED_VARS
		OpenMPT_LIBRARY OpenMPT_INCLUDE_DIR)

mark_as_advanced(OpenMPT_LIBRARY OpenMPT_INCLUDE_DIR)

set(OpenMPT_LIBRARIES ${OpenMPT_LIBRARY})
set(OpenMPT_INCLUDE_DIRS ${OpenMPT_INCLUDE_DIR})

if (OpenMPT_FOUND AND NOT TARGET OpenMPT::OpenMPT)
	add_library(OpenMPT::OpenMPT UNKNOWN IMPORTED)
	set_target_properties(OpenMPT::OpenMPT PROPERTIES
		IMPORTED_LOCATION "${OpenMPT_LIBRARY}"
		INTERFACE_COMPILE_OPTIONS "${PC_OPENMPT_CFLAGS_OTHER}"
		INTERFACE_INCLUDE_DIRECTORIES "${OpenMPT_INCLUDE_DIR}"
	)
endif()
