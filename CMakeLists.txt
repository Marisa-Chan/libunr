cmake_minimum_required (VERSION 3.7)
project (UNR CXX)

# Build options
# Choose: Debug Release RelWithDebInfo
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set (UNR_VER_MAJOR 0)
set (UNR_VER_MINOR 2)
set (UNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (UNR_INC ${UNR_ROOT}/include)
set (UNR_SRC ${UNR_ROOT}/src)

#Symlink Macro
macro(install_symlink filepath sympath)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${sympath})")
    install(CODE "message(\"-- Created symlink: ${sympath} -> ${filepath}\")")
endmacro(install_symlink)

#System Include Dir
if ( WIN32 )
  set (SYS_INC_PATH C:\\MinGW\\include)
  message(${SYS_INC_PATH})
else()
  set (SYS_INC_PATH /usr/include)
endif()

set (TARGET_PLATFORM "Linux" CACHE STRING "The target platform that is being built for") 

option( BUILD32 "Build32" OFF )

#Check for libxstl, Use <> by default, if not found, use ../xstl

#Headers
set (XSTL_INC   "${SYS_INC_PATH}/libxstl")

if( NOT EXISTS ${XSTL_INC} )
    set (XSTL_INC   "${UNR_ROOT}/../xstl/include")

    if( NOT EXISTS ${XSTL_INC} )
    message( FATAL_ERROR "Libunr Requires libxstl: library headers not found in ${SYS_INC_PATH}/libxstl or ../xstl/include" )
    endif()

endif()

#DLL/SO

#64-bit
if ( NOT BUILD32 )
    find_library( XSTL_LIB_PATH 
    NAMES xstl libxstl 
    HINTS "/usr/local/lib64/libxstl" "/usr/lib64/libxstl" "C:\\Program Files\\libxstl" "${UNR_ROOT}/../xstl/" 
    )
endif()

#32-bit
if ( BUILD32 )
find_library( XSTL_LIB_PATH 
NAMES xstl libxstl 
HINTS "/usr/local/lib32/libxstl" "/usr/lib32/libxstl" "C:\\Program Files (x86)\\libxstl" "${UNR_ROOT}/../xstl" 
)
endif()

if( NOT XSTL_LIB_PATH )
    message( FATAL_ERROR "Libunr Requires libxstl: library file not found!" )
endif()

add_library( xstl SHARED IMPORTED )
set_target_properties( xstl PROPERTIES IMPORTED_LOCATION "${XSTL_LIB_PATH}" )

if ( TARGET_PLATFORM STREQUAL "Windows" )
  add_definitions("-DLIBUNR_WIN32" "-Wno-invalid-offsetof")
elseif ( TARGET_PLATFORM STREQUAL "Linux" )
  add_definitions("-DLIBUNR_LINUX")
  install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/libunr)
  install(FILES ${UNR_ROOT}/res/DefaultLibunr.ini DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/libunr)
else()
  message( FATAL_ERROR, "Unknown target platform: " ${TARGET_PLATFORM} )
endif()

set (SOURCES ${UNR_SRC}/Core/FConfig.cpp
             ${UNR_SRC}/Core/FUtil.cpp
             ${UNR_SRC}/Core/UAudio.cpp
             ${UNR_SRC}/Core/UCanvas.cpp
             ${UNR_SRC}/Core/UClass.cpp
             ${UNR_SRC}/Core/UConsole.cpp
             ${UNR_SRC}/Core/UEngine.cpp
             ${UNR_SRC}/Core/UFire.cpp
             ${UNR_SRC}/Core/ULocale.cpp
             ${UNR_SRC}/Core/ULodMesh.cpp
             ${UNR_SRC}/Core/ULevel.cpp
             ${UNR_SRC}/Core/UMesh.cpp
             ${UNR_SRC}/Core/UModel.cpp
             ${UNR_SRC}/Core/UMusic.cpp
             ${UNR_SRC}/Core/UNet.cpp
             ${UNR_SRC}/Core/UObject.cpp
             ${UNR_SRC}/Core/UPackage.cpp
             ${UNR_SRC}/Core/UPlayer.cpp
             ${UNR_SRC}/Core/UPrimitive.cpp
             ${UNR_SRC}/Core/UProperty.cpp
             ${UNR_SRC}/Core/URender.cpp
             ${UNR_SRC}/Core/USkelAnim.cpp
             ${UNR_SRC}/Core/USkelMesh.cpp
             ${UNR_SRC}/Core/USound.cpp
             ${UNR_SRC}/Core/UStaticClassInit.cpp
             ${UNR_SRC}/Core/USystem.cpp
             ${UNR_SRC}/Core/UTexture.cpp
             ${UNR_SRC}/Core/UViewport.cpp
             ${UNR_SRC}/Actors/AActor.cpp
             ${UNR_SRC}/Actors/ABrush.cpp
             ${UNR_SRC}/Actors/ADecal.cpp
             ${UNR_SRC}/Actors/ADynamicZoneInfo.cpp
             ${UNR_SRC}/Actors/AGameInfo.cpp
             ${UNR_SRC}/Actors/AHUD.cpp
             ${UNR_SRC}/Actors/AInternetLink.cpp
             ${UNR_SRC}/Actors/AInventory.cpp
             ${UNR_SRC}/Actors/ANavigationPoint.cpp
             ${UNR_SRC}/Actors/APawn.cpp
             ${UNR_SRC}/Actors/APlayerPawn.cpp
             ${UNR_SRC}/Actors/AProjector.cpp
             ${UNR_SRC}/Actors/AReplicationInfo.cpp
             ${UNR_SRC}/Actors/AStatLog.cpp
             ${UNR_SRC}/Actors/ATimeDemo.cpp
             ${UNR_SRC}/Actors/AWeapon.cpp
             ${UNR_SRC}/Actors/AZoneInfo.cpp

)

include_directories (${UNR_INC} ${XSTL_INC})
add_definitions("-std=c++11" "-DXSTL_ARRAY_STD" "-DXSTL_STRING_STD"
  "-DXSTL_FILESTREAM_STD" "-DXSTL_STACK_STD" "-DXSTL_MAP_STD"
  "-DXSTL_QUEUE_STD" "-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"" )
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})
add_definitions ("-Wno-attributes")
set_source_files_properties( tags PROPERTIES GENERATED true )
add_custom_target( tags COMMAND ctags --c++-kinds=+p --fields=+iaS --extra=+q
 --language-force=C++ --recurse=yes . WORKING_DIRECTORY ${UNR_ROOT} )
add_library( unr SHARED ${SOURCES} tags )
target_link_libraries(unr xstl)

# Debug symbols & optimization levels
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions("-O0" "-g")
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
  if ( TARGET_PLATFORM STREQUAL "Windows" )
    install(TARGETS unr DESTINATION "${WINDOWS_32_INSTALL_PATH}\\libunr")
  elseif ( TARGET_PLATFORM STREQUAL "Linux" )
    install(TARGETS unr DESTINATION lib32/libunr)
  endif()

else()

    if ( TARGET_PLATFORM STREQUAL "Windows" )
        install(TARGETS unr DESTINATION libunr)
    elseif ( TARGET_PLATFORM STREQUAL "Linux" )
        install(TARGETS unr DESTINATION lib64/libunr)
    endif()

endif()

