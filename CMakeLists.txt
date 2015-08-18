cmake_minimum_required(VERSION 3.2)
project(luazip)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

# Define how to build zip.so:
include_directories(${LIBZIP_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
link_directories(${LIBZIP_LIBRARY_DIR})
add_library(lzip SHARED lzip.c lzip.h)
target_link_libraries(lzip zip ${LUA_LIBRARY_PATH})
set_target_properties(lzip PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
# / build