cmake_minimum_required(VERSION 3.12)

# set the project name and version
project(dxtionary-db VERSION 1.0.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
configure_file(dxtionary_version.h.in dxtionary_version.h)

# compile all modules with warning enable
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -pedantic)
endif()

# this matches variables in zlib
if(MINGW)
    # suppress option -rdynamic by cross compiling zlib
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
endif()

add_subdirectory(src)
add_subdirectory(ext/sqlite3)
add_subdirectory(ext/zlib)
