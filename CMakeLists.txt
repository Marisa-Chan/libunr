cmake_minimum_required (VERSION 3.7)
project (libunr)

# Build options
# Choose: Debug Release RelWithDebInfo
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set (libunr_VER_MAJOR 0)
set (libunr_VER_MINOR 1)
set (LIBUNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (LIBUNR_INC ${LIBUNR_ROOT}/include)
set (LIBUNR_SRC ${LIBUNR_ROOT}/src)
set (XSTL_ROOT  ${LIBUNR_ROOT}/../xstl)
set (XSTL_INC   ${XSTL_ROOT}/include)
set (TARGET_PLATFORM "Linux") 

option( BUILD32 "Build32" OFF )

if ( TARGET_PLATFORM STREQUAL "Windows" )
  add_definitions("-DLIBUNR_WIN32")
elseif ( TARGET_PLATFORM STREQUAL "Linux" )
  add_definitions("-DLIBUNR_LINUX")
  install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/libunr)
  install(FILES ${LIBUNR_ROOT}/res/DefaultLibunr.ini DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/libunr)
else()
  message( FATAL_ERROR, "Unknown target platform: " ${TARGET_PLATFORM} )
endif()

set (SOURCES ${LIBUNR_SRC}/FConfig.cpp
						 ${LIBUNR_SRC}/FUtil.cpp
             ${LIBUNR_SRC}/UClass.cpp
             ${LIBUNR_SRC}/UMusic.cpp
             ${LIBUNR_SRC}/USound.cpp
             ${LIBUNR_SRC}/UObject.cpp
             ${LIBUNR_SRC}/UPackage.cpp
             ${LIBUNR_SRC}/UProperty.cpp
             ${LIBUNR_SRC}/UTexture.cpp
             ${LIBUNR_SRC}/UStaticClassInit.cpp
             ${LIBUNR_SRC}/USystem.cpp
)

include_directories (${LIBUNR_INC} ${XSTL_INC} /usr/include)
add_definitions("-std=c++11" "-DXSTL_ARRAY_STD" "-DXSTL_STRING_STD"
  "-DXSTL_FILESTREAM_STD" "-DXSTL_STACK_STD" 
  "-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"" )
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})
add_definitions ("-Wno-attributes")
set_source_files_properties( tags PROPERTIES GENERATED true )
add_custom_target( tags COMMAND ctags --c++-kinds=+p --fields=+iaS --extra=+q
 --language-force=C++ --recurse=yes . WORKING_DIRECTORY ${LIBUNR_ROOT} )
add_library( libunr SHARED ${SOURCES} tags )
set_target_properties (libunr PROPERTIES PREFIX "")

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

# 32 bit support + install targets
if ( BUILD32 )
  add_definitions("-m32")
  set_target_properties (libunr PROPERTIES LINK_FLAGS "-m32")
  if ( TARGET_PLATFORM STREQUAL "Linux" )
    install(TARGETS libunr DESTINATION lib32)
  endif()
else()
  if ( TARGET_PLATFORM STREQUAL "Linux" )
    install(TARGETS libunr DESTINATION lib)
  endif()
endif()

