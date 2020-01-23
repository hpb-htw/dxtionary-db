cmake_minimum_required(VERSION 3.12)
# sqlite
set(SQLITE_SPELLFIX "${CMAKE_CURRENT_BINARY_DIR}/sqlite3_spellfix.c")
set(sqlite3_dep sqlite3.c spellfix.c extra_init.c.in)

add_library(sqlite3 STATIC
    ${SQLITE_SPELLFIX}
    sqlite3.h
)

target_include_directories(sqlite3
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(sqlite3
    PRIVATE SQLITE_THREADSAFE=0
    PRIVATE SQLITE_ENABLE_FTS4
    PRIVATE SQLITE_ENABLE_FTS5
    PRIVATE SQLITE_ENABLE_JSON1
    PRIVATE SQLITE_ENABLE_RTREE
    PRIVATE SQLITE_ENABLE_EXPLAIN_COMMENTS
    PRIVATE SQLITE_CORE
    PRIVATE SQLITE_EXTRA_INIT=extra_init
)

target_compile_options(sqlite3 PRIVATE
     -Wall -pedantic -Os
)

set(sqlite3_dep_list ${sqlite3_dep})
list(TRANSFORM sqlite3_dep_list PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
add_custom_command(OUTPUT ${SQLITE_SPELLFIX}
    DEPENDS ${sqlite_dep}
    COMMAND
        cat ${sqlite3_dep_list} > ${SQLITE_SPELLFIX}
)










