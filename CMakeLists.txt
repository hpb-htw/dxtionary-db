cmake_minimum_required(VERSION 3.12)

# set the project name and version
project(dxtionary-db VERSION 1.0.0)
set(CMAKE_VERBOSE_MAKEFILE ON)

# compile all modules with warning enable
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -pedantic)
endif()


add_subdirectory(src)
add_subdirectory(ext/sqlite3)

configure_file(dxtionary_version.h.in dxtionary_version.h)