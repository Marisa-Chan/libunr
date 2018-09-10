cmake_minimum_required (VERSION 3.7)
project (libunr-test)

set (SOURCES TestMain.cpp)

set (XSTL_ROOT   ${CMAKE_CURRENT_SOURCE_DIR}/../../xstl)
set (XSTL_INC    ${XSTL_ROOT}/include)
set (LIBUNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../libunr)
set (LIBUNR_INC  ${LIBUNR_ROOT}/include)

include_directories (${XSTL_INC} ${LIBUNR_INC} /usr/include)
link_directories(${LIBUNR_ROOT} ../)

add_executable (libunr-test ${SOURCES})
target_link_libraries(libunr-test "xstl" "unr") # kind of a hack but w/e

add_definitions ("-Wno-attributes" "-DXSTL_ARRAY_STD" "-DXSTL_STRING_STD"
  "-DXSTL_FILESTREAM_STD" "-DXSTL_STACK_STD" "-g")
