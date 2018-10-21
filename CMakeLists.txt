cmake_minimum_required (VERSION 3.7)
project (UNR CXX)

# Build options
# Choose: Debug Release RelWithDebInfo
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set (UNR_VER_MAJOR 0)
set (UNR_VER_MINOR 1)
set (UNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (UNR_INC ${UNR_ROOT}/include)
set (UNR_SRC ${UNR_ROOT}/src)
set (XSTL_ROOT  ${UNR_ROOT}/../xstl)
set (XSTL_INC   ${XSTL_ROOT}/include)
set (TARGET_PLATFORM "Linux" CACHE STRING "The target platform that is being built for") 

option( BUILD32 "Build32" OFF )

if ( TARGET_PLATFORM STREQUAL "Windows" )
  add_definitions("-DLIBUNR_WIN32")
elseif ( TARGET_PLATFORM STREQUAL "Linux" )
  add_definitions("-DLIBUNR_LINUX")
  install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/libunr)
  install(FILES ${UNR_ROOT}/res/DefaultLibunr.ini DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/libunr)
else()
  message( FATAL_ERROR, "Unknown target platform: " ${TARGET_PLATFORM} )
endif()

set (SOURCES ${UNR_SRC}/FConfig.cpp
             ${UNR_SRC}/FUtil.cpp
             ${UNR_SRC}/UClass.cpp
             ${UNR_SRC}/UMusic.cpp
             ${UNR_SRC}/USound.cpp
             ${UNR_SRC}/UObject.cpp
             ${UNR_SRC}/UPackage.cpp
             ${UNR_SRC}/UProperty.cpp
             ${UNR_SRC}/UTexture.cpp
             ${UNR_SRC}/UStaticClassInit.cpp
             ${UNR_SRC}/USystem.cpp
)

include_directories (${UNR_INC} ${XSTL_INC} /usr/include)
add_definitions("-std=c++11" "-DXSTL_ARRAY_STD" "-DXSTL_STRING_STD"
  "-DXSTL_FILESTREAM_STD" "-DXSTL_STACK_STD" 
  "-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"" )
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})
add_definitions ("-Wno-attributes")
set_source_files_properties( tags PROPERTIES GENERATED true )
add_custom_target( tags COMMAND ctags --c++-kinds=+p --fields=+iaS --extra=+q
 --language-force=C++ --recurse=yes . WORKING_DIRECTORY ${UNR_ROOT} )
add_library( unr SHARED ${SOURCES} tags )
target_link_libraries(unr "xstl")

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
  set_target_properties (unr PROPERTIES LINK_FLAGS "-m32")
  if ( TARGET_PLATFORM STREQUAL "Linux" )
    install(TARGETS unr DESTINATION lib32)
  endif()
else()
  if ( TARGET_PLATFORM STREQUAL "Linux" )
    install(TARGETS unr DESTINATION lib)
  endif()
endif()

