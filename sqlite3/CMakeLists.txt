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
    PRIVATE SQLITE_EXTRA_INIT=extra_init
)

target_compile_options(sqlite3 PRIVATE
     -Wall -pedantic -Os
)


# generate sqlite3_spellfix.c
# It could be nice if I have cmake 3.12 or newer installed
# => use list(TRANSFORM ...) to append dir to each of
# sqlite3.c spellfix.c extra_init.in.c

set(sqlite3_dep sqlite3.c spellfix.c extra_init.in.c)
function(prepend var prefix file_list)
   set(listVar "")
   foreach(f ${file_list})
      list(APPEND listVar "${prefix}/${f}")
   endforeach(f)
   set(${var} "${listVar}" PARENT_SCOPE)
endfunction(prepend)
prepend(sqlite3_dep_list ${CMAKE_CURRENT_SOURCE_DIR} "${sqlite3_dep}")
# list(TRANSFORM sqlite3_dep PREPEND ${CMAKE_CURRENT_SOURCE_DIR}) # would do the job
add_custom_command(OUTPUT ${SQLITE_SPELLFIX}
    DEPENDS ${sqlite_dep}
    COMMAND
        cat ${sqlite3_dep_list} > ${SQLITE_SPELLFIX}
)










