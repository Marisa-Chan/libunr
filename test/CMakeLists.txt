cmake_minimum_required (VERSION 3.7)
project (libunr-test CXX)

# Build options
# Choose: Debug Release RelWithDebInfo
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()
option (BUILD32 "BUILD32" OFF)

set (SOURCES TestMain.cpp)

set (XSTL_ROOT   ${CMAKE_CURRENT_SOURCE_DIR}/../../xstl)
set (XSTL_INC    ${XSTL_ROOT}/include)
set (LIBUNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../libunr)
set (LIBUNR_INC  ${LIBUNR_ROOT}/include)

include_directories (${XSTL_INC} ${LIBUNR_INC} /usr/include)
link_directories(${LIBUNR_ROOT} ../)

add_executable (libunr-test ${SOURCES})
find_library(LIBUNR unr)
find_library(XSTL  xstl)
target_link_libraries(libunr-test ${XSTL})
target_link_libraries(libunr-test ${LIBUNR})

add_definitions ("-Wno-attributes" "-DXSTL_ARRAY_STD" "-DXSTL_STRING_STD"
  "-DXSTL_FILESTREAM_STD" "-DXSTL_STACK_STD" )

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

if ( BUILD32 )
  add_definitions ("-m32")
  set_target_properties (libunr-test PROPERTIES LINK_FLAGS "-m32")
endif()
