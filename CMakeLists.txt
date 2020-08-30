cmake_minimum_required (VERSION 3.10)
project (UNR CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

set(UNR_VER_MAJOR 0)
set(UNR_VER_MINOR 2)
set(UNR_VER_PATCH 1)
set(UNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(UNR_INC ${UNR_ROOT}/include)
set(UNR_SRC ${UNR_ROOT}/src)

option(BUILD_CYGWIN "Build_Cygwin" OFF)
option(BUILD_DEUSEX "DeusEx support" OFF)
option(BUILD_RUNE "Rune support" OFF)
option(BUILD_GME "GME support" OFF)
option(BUILD_OPENMPT "OpenMPT support" ON)
option(BUILD_VORBISFILE "Vorbisfile support" OFF)
option(BUILD_MPG123 "Mpg123 support" OFF)

add_library(unr SHARED)
add_library(Unr::Unr ALIAS unr)

find_package(X11)
find_package(Threads REQUIRED)

if (BUILD_MPG123)
	find_package(Mpg123 REQUIRED)
	target_link_libraries(unr
		PUBLIC
			Mpg123::Mpg123
	)
	target_compile_definitions(unr
		PUBLIC
			"BUILD_MPG123=1"
	)
endif()

if (BUILD_VORBISFILE)
	find_package(Vorbisfile REQUIRED)
	target_link_libraries(unr
		PUBLIC
			Vorbisfile::Vorbisfile
	)
	target_compile_definitions(unr
		PUBLIC
			"BUILD_VORBISFILE=1"
	)
endif()

if (BUILD_OPENMPT)
	find_package(OpenMPT REQUIRED)
	target_link_libraries(unr
		PUBLIC
			OpenMPT::OpenMPT
	)
	target_compile_definitions(unr
		PUBLIC
			"BUILD_OPENMPT=1"
	)
endif()

if (BUILD_GME)
	find_package(Gme REQUIRED)
	target_link_libraries(unr
		PUBLIC
			Gme::Gme
	)
	target_compile_definitions(unr
		PUBLIC
			"BUILD_GME=1"
	)
endif()

add_definitions("-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"" )
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})

target_sources(unr
	PRIVATE
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

target_sources(unr
	PRIVATE
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

target_sources(unr
	PRIVATE
		${UNR_SRC}/Editor/UEditorEngine.cpp
		${UNR_SRC}/Editor/UExporter.cpp
		${UNR_SRC}/Editor/UFactory.cpp
)

target_sources(unr
	PRIVATE
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

if (X11_FOUND)
	target_include_directories(unr
		PUBLIC
			${X11_INCLUDE_DIR}
	)

	target_compile_definitions(unr
		PUBLIC
			"BUILD_X11=1"
	)

	target_sources(unr
		PRIVATE
			${UNR_SRC}/Engine/UX11Client.cpp
			${UNR_SRC}/Engine/UX11Viewport.cpp
	)

	target_link_libraries(unr
		PUBLIC
			${X11_LIBRARIES}
	)
endif()

if (DEFINED SDL2_LIBRARIES)
	target_include_directories(unr
		PUBLIC
			${SDL2_INCLUDE_DIRS}
	)

	target_compile_definitions(unr
		PUBLIC
			"LIBUNR_SDL2=1"
			"BUILD_SDL2=1"
	)

	target_sources(unr
		PRIVATE
			${UNR_SRC}/Engine/USdlClient.cpp
			${UNR_SRC}/Engine/USdlViewport.cpp
	)

	target_link_libraries(unr
		PUBLIC
			${SDL2_LIBRARIES}
	)
endif()

if (BUILD_DEUSEX)
	target_sources(unr
		PRIVATE
			${UNR_SRC}/DeusEx/ACameraPoint.cpp
			${UNR_SRC}/DeusEx/UEventManager.cpp
	)

	target_compile_definitions(unr
		PUBLIC
			"BUILD_DEUSEX=1"
	)
endif()

if (BUILD_RUNE)
	target_sources(unr
		PRIVATE
			${UNR_SRC}/Rune/ARuneActor.cpp
	)

	target_compile_definitions(unr
		PUBLIC
			"BUILD_RUNE=1"
	)
endif()

target_sources(unr
	PRIVATE
		${UNR_SRC}/HPSS/AParticleFX.cpp
		${UNR_SRC}/HPSS/UParticleList.cpp
)

target_sources(unr
	PRIVATE
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

if (APPLE)
	target_compile_definitions(unr
		PRIVATE
			"_XOPEN_SOURCE=600"
	)
endif()

if (WIN32)
	target_sources(unr
		PRIVATE
			${UNR_SRC}/Engine/UWindowsClient.cpp
			${UNR_SRC}/Engine/UWindowsViewport.cpp
	)

	target_link_libraries(unr
		PUBLIC
			winmm.lib
	)
endif()

if (NOT WIN32)
	# XXX: CMake <=3.13 does not have target_link_options()
	# We are abusing target_link_libraries() to emulate its functionality
	set(LIBUNR_EMULATED_TARGET_LINK_OPTIONS "-Wl,-no-undefined")

	find_package(Uuid REQUIRED)

	target_link_libraries(unr
		PUBLIC
			Uuid::Uuid
		PRIVATE
			dl
	)
endif()

target_compile_features(unr PRIVATE cxx_std_11)

target_include_directories(unr
	PUBLIC
		$<BUILD_INTERFACE:${UNR_INC}>
		$<INSTALL_INTERFACE:include/unr>
)

target_compile_definitions(unr
	PRIVATE
		"LIBUNR_EXPORTS=1"
)

target_link_libraries(unr
	PUBLIC
		Threads::Threads
	PRIVATE
		"${LIBUNR_EMULATED_TARGET_LINK_OPTIONS}"
)

set_target_properties(unr PROPERTIES EXPORT_NAME Unr)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/UnrConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/UnrConfig.cmake
	INSTALL_DESTINATION lib/cmake/Unr
)

install(TARGETS unr
	EXPORT unr-export
	LIBRARY
		DESTINATION lib
	PUBLIC_HEADER
		DESTINATION include/unr
	INCLUDES DESTINATION include/unr
)

install(DIRECTORY include/
	DESTINATION include/unr
)

if (UNIX)
  install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/libunr)
  install(FILES ${UNR_ROOT}/res/DefaultLibunr.ini DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/libunr)
endif()

install(EXPORT unr-export
	FILE
		UnrTargets.cmake
	NAMESPACE
		Unr::
	DESTINATION
		lib/cmake/Unr
)

install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/UnrConfig.cmake
		cmake/Modules/FindMpg123.cmake
		cmake/Modules/FindOpenMPT.cmake
		cmake/Modules/FindGme.cmake
		cmake/Modules/FindUuid.cmake
		cmake/Modules/FindVorbisfile.cmake
	DESTINATION
		lib/cmake/Unr
)
