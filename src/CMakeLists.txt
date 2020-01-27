cmake_minimum_required(VERSION 3.12)

# set the project name and version
project(dxtionary-db)
# TODO: remve these two commands after test
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add the executable
## dxtionary-db
add_executable(dxtionary-db
    dxtionary_db.cpp
)
target_include_directories(dxtionary-db PUBLIC
    "${PROJECT_BINARY_DIR}"
)
target_link_libraries(dxtionary-db PUBLIC
    sqlite3 -ldl -lm
)

## decompress-plain-db
add_executable(decompress-plain-db
    decompress_plain_db.cpp
    xz_stream.cpp
    xz_stream.h
)

target_link_libraries(decompress-plain-db
    -llzma
)

## zlibexample.c (Usage example; remove after understand how zlib works)
add_executable(zlibexample
    zlibexample.c
)

target_link_libraries(zlibexample
    zlibstatic
)

## gz-stream-file-test
add_library(gz_file_stream
    STATIC
        gz_file_stream.cpp
        gz_file_stream.hpp
)

target_link_libraries(gz_file_stream
    PRIVATE
        zlibstatic
)

add_executable(gz-file-stream-test
    gz_file_stream_test.cpp
)

target_link_libraries(gz-file-stream-test
    PRIVATE
        gz_file_stream
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
