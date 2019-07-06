cmake_minimum_required (VERSION 3.7)
project (UNR CXX)

# Build options
# Choose: Debug Release RelWithDebInfo
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set (UNR_VER_MAJOR 0)
set (UNR_VER_MINOR 2)
set (UNR_VER_PATCH 1)
set (UNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (UNR_INC ${UNR_ROOT}/include)
set (UNR_SRC ${UNR_ROOT}/src)

#Symlink Macro
macro(install_symlink filepath sympath)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${sympath})")
    install(CODE "message(\"-- Created symlink: ${sympath} -> ${filepath}\")")
endmacro(install_symlink)

set (TARGET_PLATFORM "POSIX" CACHE STRING "The target platform that is being built for") 

option( BUILD32 "Build32" OFF )
option( BUILD_CYGWIN "Build_Cygwin" OFF )

#System Include Dir
if ( TARGET_PLATFORM STREQUAL "Windows" )
  if ( BUILD32 )
	set (SYS_INC_PATH C:\\MinGW\\include)
  else()
	set (SYS_INC_PATH C:\\Program\ Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\include)
  endif()
  message(${SYS_INC_PATH})
else()
  set (SYS_INC_PATH /usr/include)
endif()

#Check for libxstl, Use <> by default, if not found, use ../xstl

if ( TARGET_PLATFORM STREQUAL "Windows" )
  add_definitions("-Wno-invalid-offsetof")
elseif ( TARGET_PLATFORM STREQUAL "POSIX" )
  install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/libunr)
  install(FILES ${UNR_ROOT}/res/DefaultLibunr.ini DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/libunr)
else()
  message( FATAL_ERROR, "Unknown target platform: " ${TARGET_PLATFORM} )
endif()

set (SOURCES ${UNR_SRC}/Core/FConfig.cpp
             ${UNR_SRC}/Core/FLogFile.cpp
             ${UNR_SRC}/Core/FUtil.cpp
             ${UNR_SRC}/Core/UAudio.cpp
             ${UNR_SRC}/Core/UCanvas.cpp
             ${UNR_SRC}/Core/UClass.cpp
             ${UNR_SRC}/Core/UConsole.cpp
             ${UNR_SRC}/Core/UEngine.cpp
             ${UNR_SRC}/Core/UFire.cpp
             ${UNR_SRC}/Core/UGesture.cpp
             ${UNR_SRC}/Core/ULocale.cpp
             ${UNR_SRC}/Core/ULodMesh.cpp
             ${UNR_SRC}/Core/ULevel.cpp
             ${UNR_SRC}/Core/UMath.cpp
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
             ${UNR_SRC}/Core/UStaticMesh.cpp
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
             ${UNR_SRC}/DeusEx/UEventManager.cpp
             ${UNR_SRC}/DeusEx/ACameraPoint.cpp
             ${UNR_SRC}/Rune/ARuneActor.cpp
             ${UNR_SRC}/HPSS/UParticleList.cpp
             ${UNR_SRC}/HPSS/AParticleFX.cpp
)

include_directories (${UNR_INC} ${XSTL_INC})
add_definitions("-fmax-errors=10" "-std=c++11" "-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"" )
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})
add_definitions ("-Wno-attributes")
set_source_files_properties( tags PROPERTIES GENERATED true )
add_custom_target( tags COMMAND ctags --c++-kinds=+p --fields=+iaS --extra=+q
 --language-force=C++ --recurse=yes . WORKING_DIRECTORY ${UNR_ROOT} )
add_library( unr SHARED ${SOURCES} tags )

if ( TARGET_PLATFORM STREQUAL "Windows" )
	target_link_libraries(unr)
else()
	target_link_libraries(unr dl)
endif()

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
  elseif ( TARGET_PLATFORM STREQUAL "POSIX" )
    install(TARGETS unr DESTINATION lib32/libunr)
  endif()

else()

    if ( TARGET_PLATFORM STREQUAL "Windows" )
        install(TARGETS unr DESTINATION libunr)
    elseif ( TARGET_PLATFORM STREQUAL "POSIX" )
        install(TARGETS unr DESTINATION lib64/libunr)
    endif()

endif()

