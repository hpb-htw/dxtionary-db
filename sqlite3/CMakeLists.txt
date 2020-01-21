# sqlite
add_library(sqlite3 STATIC
    sqlite3.c
    sqlite3.h
)

target_include_directories(sqlite3
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(sqlite3
    INTERFACE SQLITE_THREADSAFE=0
    INTERFACE SQLITE_ENABLE_FTS4
    INTERFACE SQLITE_ENABLE_FTS5
    INTERFACE SQLITE_ENABLE_JSON1
    INTERFACE SQLITE_ENABLE_RTREE
    INTERFACE SQLITE_ENABLE_EXPLAIN_COMMENTS
    INTERFACE SQLITE_CORE
)

target_compile_options(sqlite3 PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
              -Wall -pedantic -Os>
    $<$<CXX_COMPILER_ID:MSVC>:
              /W4>
)



# spellfix
add_library(spellfix STATIC
    spellfix.c
    sqlite3ext.h
)


target_include_directories(spellfix
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(spellfix PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
              -Wall -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:
              /W4>
)


