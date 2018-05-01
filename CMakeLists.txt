cmake_minimum_required (VERSION 3.7)
project (libunr)

set (libunr_VER_MAJOR 0)
set (libunr_VER_MINOR 1)
set (LIBUNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (LIBUNR_INC ${LIBUNR_ROOT}/include)
set (LIBUNR_SRC ${LIBUNR_ROOT}/src)

set (SOURCES ${LIBUNR_SRC}/FArchiveFile.cpp
             ${LIBUNR_SRC}/FCompactIndex.cpp
             ${LIBUNR_SRC}/FMemory.cpp
             ${LIBUNR_SRC}/FName.cpp
             ${LIBUNR_SRC}/FString.cpp
             ${LIBUNR_SRC}/UClass.cpp
             ${LIBUNR_SRC}/UMusic.cpp
             ${LIBUNR_SRC}/USound.cpp
             ${LIBUNR_SRC}/UObject.cpp
             ${LIBUNR_SRC}/UPackage.cpp
             ${LIBUNR_SRC}/UProperty.cpp
             ${LIBUNR_SRC}/UTexture.cpp
             ${LIBUNR_SRC}/UStaticClassInit.cpp
)

include_directories (${LIBUNR_INC} /usr/include)
add_library (libunr SHARED ${SOURCES})
add_definitions("-std=c++11")
add_definitions ("-Wno-attributes" "-g")
set_target_properties (libunr PROPERTIES PREFIX "")
