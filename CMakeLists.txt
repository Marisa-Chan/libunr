cmake_minimum_required (VERSION 3.10)
project (UNR CXX)

set(CMAKE_CXX_STANDARD 11)

set( CMAKE_MODULE_PATH cmake/Modules ${CMAKE_MODULE_PATH} )

set (UNR_VER_MAJOR 0)
set (UNR_VER_MINOR 2)
set (UNR_VER_PATCH 1)
set (UNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (UNR_INC ${UNR_ROOT}/include)
set (UNR_SRC ${UNR_ROOT}/src)

option( BUILD_CYGWIN "Build_Cygwin" OFF )
option( BUILD_DEUSEX "DeusEx support" OFF )
option( BUILD_RUNE "Rune support" OFF )

if ( WIN32 )
  add_definitions("-Wno-invalid-offsetof")
endif()

if ( UNIX )
  install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/libunr)
  install(FILES ${UNR_ROOT}/res/DefaultLibunr.ini DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/libunr)
	install(FILES ${UNR_ROOT}/res/OpenUE.u DESTINATION 
		${CMAKE_INSTALL_PREFIX}/share/libunr)
endif()

set (ACTOR_SOURCES
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

set (CORE_SOURCES
     ${UNR_SRC}/Core/UClass.cpp
     ${UNR_SRC}/Core/ULocale.cpp
     ${UNR_SRC}/Core/UMath.cpp
     ${UNR_SRC}/Core/UNativeModule.cpp
     ${UNR_SRC}/Core/UObject.cpp
     ${UNR_SRC}/Core/UPackage.cpp
     ${UNR_SRC}/Core/UPrimitive.cpp
     ${UNR_SRC}/Core/UProperty.cpp
     ${UNR_SRC}/Core/UStaticClassInit.cpp
     ${UNR_SRC}/Core/USystem.cpp
)

set (DEUSEX_SOURCES
     ${UNR_SRC}/DeusEx/ACameraPoint.cpp
	 ${UNR_SRC}/DeusEx/UEventManager.cpp
)

set (EDITOR_SOURCES
	${UNR_SRC}/Editor/UEditorEngine.cpp
	${UNR_SRC}/Editor/UExporter.cpp
	${UNR_SRC}/Editor/UFactory.cpp
)

set (ENGINE_SOURCES
	${UNR_SRC}/Engine/UAudio.cpp
	${UNR_SRC}/Engine/UCanvas.cpp
	${UNR_SRC}/Engine/UConsole.cpp
	${UNR_SRC}/Engine/UEngine.cpp
	${UNR_SRC}/Engine/UFire.cpp
	${UNR_SRC}/Engine/UGesture.cpp
	${UNR_SRC}/Engine/ULevel.cpp
	${UNR_SRC}/Engine/ULodMesh.cpp
	${UNR_SRC}/Engine/UMesh.cpp
	${UNR_SRC}/Engine/UModel.cpp
	${UNR_SRC}/Engine/UMusic.cpp
	${UNR_SRC}/Engine/UNet.cpp
	${UNR_SRC}/Engine/UPlayer.cpp
	${UNR_SRC}/Engine/URender.cpp
	${UNR_SRC}/Engine/USkelAnim.cpp
	${UNR_SRC}/Engine/USkelMesh.cpp
	${UNR_SRC}/Engine/USound.cpp
	${UNR_SRC}/Engine/UStaticMesh.cpp
	${UNR_SRC}/Engine/UTexture.cpp
	${UNR_SRC}/Engine/UViewport.cpp
)

set (WINDOWS_ENGINE_SOURCES
	${UNR_SRC}/Engine/UWindowsClient.cpp
	${UNR_SRC}/Engine/UWindowsViewport.cpp
)

set (X11_ENGINE_SOURCES
	${UNR_SRC}/Engine/UX11Client.cpp
	${UNR_SRC}/Engine/UX11Viewport.cpp
)

set (HPSS_SOURCES
	${UNR_SRC}/HPSS/AParticleFX.cpp
	${UNR_SRC}/HPSS/UParticleList.cpp
)

set (RUNE_SOURCES
	${UNR_SRC}/Rune/ARuneActor.cpp
)

set (UTIL_SOURCES
	${UNR_SRC}/Util/FBacktrace.cpp
	${UNR_SRC}/Util/FConfig.cpp
	${UNR_SRC}/Util/FFileArchive.cpp
	${UNR_SRC}/Util/FGuid.cpp
	${UNR_SRC}/Util/FHash.cpp
	${UNR_SRC}/Util/FLogFile.cpp
	${UNR_SRC}/Util/FMemory.cpp
	${UNR_SRC}/Util/FName.cpp
	${UNR_SRC}/Util/FString.cpp
)

find_package(X11)
find_package(Mpg123 REQUIRED)
find_package(Vorbisfile REQUIRED)
find_package(Dumb REQUIRED)
find_package(Gme REQUIRED)

include_directories(
	${UNR_INC}
	${LIBMPG123_INCLUDE_DIRS}
	${LIBGME_INCLUDE_DIRS}
	${LIBDUMB_INCLUDE_DIRS}
	${X11_INCLUDE_DIR}
	${VORBISFILE_INCLUDE_DIRS}
)

add_definitions("-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"" )
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})

set_source_files_properties( tags PROPERTIES GENERATED true )
add_custom_target( tags COMMAND ctags --c++-kinds=+p --fields=+iaS --extra=+q
 --language-force=C++ --recurse=yes . WORKING_DIRECTORY ${UNR_ROOT} )

set(LIBUNR_SOURCES
	${ACTOR_SOURCES}
	${CORE_SOURCES}
	${EDITOR_SOURCES}
	${ENGINE_SOURCES}
	${HPSS_SOURCES}
    ${UTIL_SOURCES}
)

if ( WIN32 )
	set(LIBUNR_SOURCES ${LIBUNR_SOURCES} ${WINDOWS_ENGINE_SOURCES} )
endif()

if ( APPLE )
	add_definitions("-D_XOPEN_SOURCE=600")
endif()

if ( X11_FOUND )
	set(LIBUNR_SOURCES ${LIBUNR_SOURCES} ${X11_ENGINE_SOURCES} )
	add_definitions("-DBUILD_X11=1")
endif()

if ( BUILD_DEUSEX )
	set(LIBUNR_SOURCES ${LIBUNR_SOURCES} ${DEUSEX_SOURCES} )
	add_definitions("-DBUILD_DEUSEX=1")
endif()

if ( BUILD_RUNE )
	set(LIBUNR_SOURCES ${LIBUNR_SOURCES} ${RUNE_SOURCES} )
	add_definitions("-DBUILD_RUNE=1")
endif()

add_library(unr SHARED
	${LIBUNR_SOURCES}
)

target_link_libraries(unr
	dl
	pthread
	uuid
	${LIBMPG123_LIBRARIES}
	${LIBGME_LIBRARIES}
	${LIBDUMB_LIBRARIES}
	${LIBVORBISFILE_LIBRARIES}
	${X11_LIBRARIES}
)

install(TARGETS unr LIBRARY
        DESTINATION lib)
