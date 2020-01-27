cmake_minimum_required(VERSION 3.12)

# set the project name and version
project(dxtionary-db)

# add the executable
add_executable(dxtionary-db
    dxtionary_db.cpp
)
target_include_directories(dxtionary-db PUBLIC
    "${PROJECT_BINARY_DIR}"
)
target_link_libraries(dxtionary-db PUBLIC
    sqlite3 -ldl -lm
)

add_executable(decompress-plain-db
    decompress_plain_db.cpp
    xz_stream.cpp
    xz_stream.h
)

target_link_libraries(decompress-plain-db
    -llzma
)


# Simple test; run with ctest

enable_testing()

# does the application run
add_test(NAME Usage COMMAND dxtionary-db)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*DATABASE SQL-STATEMENT"
)


# define a function to simplify adding tests
function(do_test name target select result)
    add_test(NAME Comp_${name}
        COMMAND ${target} "/tmp/test.db" ${select}
    )
    message(select)
    set_tests_properties( Comp_${name}
        PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

do_test(editdist3 dxtionary-db "SELECT editdist3(\"Julia\", \"Jullian\") as dist;" "dist = 200")
do_test(json_quote dxtionary-db "SELECT json_quote(json_object(\"name\", \"Jull\nian\")) as json;" "json =.*")
# do_test(sql_error dxtionary-db "CREATE TABLE IF NOT EXIST wiktionary(a TEXT, b TEXT, c TEXT); INSERT INTO wiktionary VALUES('D', 'DD', 'Das Buchstaben D', 'X')" "SQL error: table wiktionary has 3 columns but 4 values were supplied")
