# sqlite
set(SQLITE_SPELLFIX "sqlite3_spellfix.c")
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
    PRIVATE SQLITE_EXTRA_INIT=core_init
)

target_compile_options(sqlite3 PRIVATE
     -Wall -pedantic -Os
)


# generate sqlite3_spellfix.c
add_custom_command(OUTPUT ${SQLITE_SPELLFIX}
    DEPENDS sqlite3.c spellfix.c core_init.in.c
    COMMAND
        cat ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c ${CMAKE_CURRENT_SOURCE_DIR}/spellfix.c ${CMAKE_CURRENT_SOURCE_DIR}/core_init.in.c > ${SQLITE_SPELLFIX}
)

