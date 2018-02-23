cmake_minimum_required (VERSION 3.7)
project (libunr)

set (libunr_VER_MAJOR 0)
set (libunr_VER_MINOR 1)
set (LIBUNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (LIBUNR_INC ${LIBUNR_ROOT}/include)
set (LIBUNR_SRC ${LIBUNR_ROOT}/src)

set (HEADERS ${LIBUNR_INC}/FArchive.h
             ${LIBUNR_INC}/FArchiveFile.h
             ${LIBUNR_INC}/FMemory.h
             ${LIBUNR_INC}/FName.h
             ${LIBUNR_INC}/FString.h
             ${LIBUNR_INC}/FUtil.h
             ${LIBUNR_INC}/TArray.h
             ${LIBUNR_INC}/UPackage.h
)


set (SOURCES ${LIBUNR_SRC}/FArchiveFile.cpp
             ${LIBUNR_SRC}/FCompactIndex.cpp
             ${LIBUNR_SRC}/FMemory.cpp
             ${LIBUNR_SRC}/FName.cpp
             ${LIBUNR_SRC}/UPackage.cpp
)

include_directories (${LIBUNR_INC} /usr/include)
add_library (libunr SHARED ${SOURCES})
add_definitions ("-Wno-attributes" "-g")
set_target_properties (libunr PROPERTIES PREFIX "")

install(TARGETS libunr DESTINATION /usr/local/lib)
