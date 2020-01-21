cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(dxtionary-db VERSION 1.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set( CMAKE_VERBOSE_MAKEFILE ON)


option(SQLITE_THREADSAFE   "Use pthread library to make sqlite3 thread safe" OFF)
option(SQLITE_ENABLE_FTS4  "Extention fulltext search 4"                     ON)
option(SQLITE_ENABLE_FTS5  "Extention fulltext search 5"                     ON)
option(SQLITE_ENABLE_JSON1 "Extention Json1"                                 ON)
option(SQLITE_ENABLE_RTREE "Extension RTree"                                 ON)
option(SQLITE_ENABLE_EXPLAIN_COMMENTS "explain comments"                     ON)
option(SQLITE_CORE         "Sqlite core" ON)


configure_file(dxtionary_version.in.h dxtionary_version.h)

# add the sqlite3 library
add_subdirectory(sqlite3)
list(APPEND EXTRA_LIBS sqlite3 spellfix)




# add the executable
add_executable(dxtionary-db dxtionary-db.cpp sqlite3.c spellfix.c)
target_link_libraries(dxtionary-db PUBLIC
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        ${EXTRA_LIBS} -ldl -lm>
    $<$<CXX_COMPILER_ID:MSVC>:
        ${EXTRA_LIBS}>
)


target_include_directories(dxtionary-db PUBLIC
    "${PROJECT_BINARY_DIR}"
)
