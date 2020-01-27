cmake_minimum_required(VERSION 3.12)
# sqlite
set(SQLITE_SPELLFIX "${CMAKE_CURRENT_BINARY_DIR}/sqlite3_spellfix.c")
set(sqlite3_dep sqlite3.c spellfix.c extra_init.c.in)

add_library(sqlite3 STATIC
    ${SQLITE_SPELLFIX}
    sqlite3.h
)

target_include_directories(sqlite3
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
# S. also https://www.sqlite.org/compile.html
target_compile_definitions(sqlite3
    PRIVATE
        SQLITE_THREADSAFE=0
        SQLITE_ENABLE_FTS4
        SQLITE_ENABLE_FTS5
        SQLITE_ENABLE_JSON1
        SQLITE_ENABLE_RTREE
        SQLITE_ENABLE_EXPLAIN_COMMENTS
        SQLITE_OMIT_LOAD_EXTENSION
        SQLITE_CORE
        SQLITE_EXTRA_INIT=extra_init
)

target_compile_options(sqlite3
    PRIVATE -Os
)

function(concat sources destination)
    file(WRITE ${destination} "")
    message(STATUS "concat")
    foreach( inFile ${sources} )
        message(STATUS "        ${inFile}")
        file(READ ${inFile} temp)
        file(APPEND ${destination} "${temp}")
    endforeach()
    message(STATUS "     to ${destination}")
endfunction()


set(sqlite3_dep_list ${sqlite3_dep})
list(TRANSFORM sqlite3_dep_list PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
#concat("${sqlite3_dep_list}" ${SQLITE_SPELLFIX})

if(WIN32)
    add_custom_command(OUTPUT ${SQLITE_SPELLFIX}
        DEPENDS ${sqlite_dep}
        COMMAND
        # cat works well on almost platform, except windows :P
        type ${sqlite3_dep_list} > ${SQLITE_SPELLFIX}
    )
else()
    add_custom_command(OUTPUT ${SQLITE_SPELLFIX}
        DEPENDS ${sqlite_dep}
        COMMAND
            # cat works well on almost platform, except windows :P
            cat ${sqlite3_dep_list} > ${SQLITE_SPELLFIX}
    )
endif()









