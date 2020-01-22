# sqlite
add_library(sqlite3 STATIC
    sqlite3_spellfix.c
    sqlite3.h
)

target_include_directories(sqlite3
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(sqlite3
    PRIVATE SQLITE_THREADSAFE=0
    PRIVATE SQLITE_ENABLE_FTS4
    PRIVATE SQLITE_ENABLE_FTS5
    PRIVATE SQLITE_ENABLE_JSON1
    PRIVATE SQLITE_ENABLE_RTREE
    PRIVATE SQLITE_ENABLE_EXPLAIN_COMMENTS
    PRIVATE SQLITE_CORE
    PRIVATE SQLITE_EXTRA_INIT=core_init
)

target_compile_options(sqlite3 PRIVATE
              -Wall -pedantic -Os
)




