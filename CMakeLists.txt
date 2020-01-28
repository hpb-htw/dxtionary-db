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
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
    )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
    )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#[==[ Add googletest directly to our build. This adds the following targets:
      gtest, gtest_main, gmock and gmock_main
#]==]
add_subdirectory(
    "${CMAKE_BINARY_DIR}/googletest-src"
    "${CMAKE_BINARY_DIR}/googletest-build"
    EXCLUDE_FROM_ALL
)

#[==[ The gtest/gmock targets carry header search path dependencies
      automatically when using CMake 2.8.11 or later. Otherwise we
      have to add them here ourselves.
#]==]
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
        "${gmock_SOURCE_DIR}/include"
        )
endif()

## add unit-test code
add_subdirectory(test)