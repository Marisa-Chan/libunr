cmake_minimum_required (VERSION 3.7)
project (libunr-editor CXX)

# Build options
# Choose: Debug | Release | RelWithDebInfo
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set (LUED_VER_MAJOR 0)
set (LUED_VER_MINOR 1)

set (LUED_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (LUED_INC  ${LUED_ROOT}/include)
set (LUED_SRC  ${LUED_ROOT}/src)

set (LUNR_ROOT ${LUED_ROOT}/..)
set (LUNR_INC  ${LUNR_ROOT}/include)

set (XSTL_ROOT ${LUED_ROOT}/../../xstl)
set (XSTL_INC  ${XSTL_ROOT}/include) 

set (TARGET_PLATFORM "Linux" CACHE STRING "The target platform that is being built for")
option (BUILD32 "BUILD32" OFF)

set (SOURCES ${LUED_SRC}/UEdApp.cpp
             ${LUED_SRC}/UEdFrame.cpp
             ${LUED_SRC}/UEdMainFrame.cpp
)

include_directories (${LUED_INC} ${LUNR_INC} ${XSTL_INC} /usr/include
  /usr/include/wx-3.0)
add_definitions("-std=c++11" "-DXSTL_ARRAY_STD" "-DXSTL_STRING_STD"
  "-DXSTL_FILESTREAM_STD" "-DXSTL_STACK_STD"
  "-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"" )
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})

#TODO: Automatically call wx-config
add_definitions("-I/usr/lib64/wx/include/gtk2-unicode-3.0"
  "-I/usr/include/wx-3.0" "-D_FILE_OFFSET_BITS=64" "-DWXUSINGDLL"
  "-D__WXGTK__")

if ( WIN32 )
  add_definitions("-Wno-invalid-offsetof")
elseif ( UNIX )
  install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/libunr/editor)
  install(FILES ${LUED_ROOT}/res/DefaultLibunrEditor.ini DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/libunr/editor)
else()
  message( FATAL_ERROR, "Unknown target platform: " ${TARGET_PLATFORM} )
endif()

add_definitions ("-Wno-attributes")
set_source_files_properties( tags PROPERTIES GENERATED true )
add_custom_target( tags COMMAND ctags --c++-kinds=+p --fields=+iaS --extra=+q
  --language-force=C++ --recurse=yes . WORKING_DIRECTORY ${LUED_ROOT} )
add_executable (libunr-editor ${SOURCES})
install(TARGETS libunr-editor DESTINATION bin)

find_library(LIBUNR 
  NAMES unr
  HINTS ${LIBUNR_ROOT}
)

find_library(XSTL  
  NAMES xstl
  HINTS ${XSTL_ROOT}
)

find_library(WXBASE NAMES wx_baseu-3.0)
find_library(WXCORE NAMES wx_gtk2u_core-3.0)

target_link_libraries(libunr-editor ${XSTL})
target_link_libraries(libunr-editor ${LIBUNR})
target_link_libraries(libunr-editor ${WXBASE})
target_link_libraries(libunr-editor ${WXCORE})

# Debug symbols & optimization levels
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions("-O1" "-g")
elseif ( CMAKE_BUILD_TYPE STREQUAL "Release" )
  add_definitions("-O2")
elseif ( CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
  add_definitions("-O2" "-g")
else()
  message(FATAL_ERROR "Unknown build type: " ${CMAKE_BUILD_TYPE})
endif()

# 32 bit support
if ( BUILD32 )
  add_definitions("-m32")
endif()

