cmake_minimum_required(VERSION 3.12)

# set the project name and version
project(dxtionary-db VERSION 1.0.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

configure_file(dxtionary_version.h.in dxtionary_version.h)
message(STATUS "use toolchain file ${CMAKE_TOOLCHAIN_FILE}")

configure_file(gen-big-raw-data.py gen-big-raw-data.py COPYONLY)

# compile all modules with warning enable
if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -pedantic)
endif()

# this matches same variable in zlib and suppress compiler flag -rdynamic
if(MINGW)
    # suppress option -rdynamic by cross compiling zlib
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
endif()

# source code
add_subdirectory(src)
add_subdirectory(ext/sqlite3)
## only build necessary CPP in zlib
add_subdirectory(ext/zlib EXCLUDE_FROM_ALL)

# Unit-test
add_subdirectory(test)

