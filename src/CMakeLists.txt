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
)
target_link_libraries(decompress-plain-db
    PRIVATE
        gz_file_stream
)
add_library(gz_file_stream
    STATIC
        gz_file_stream.cpp
        gz_file_stream.hpp
)
target_link_libraries(gz_file_stream
    PRIVATE
        zlibstatic
)

## xz-stream-file-test
add_executable(xz-stream-test
    xz_stream_test.cpp
    xz_stream.cpp
    xz_stream.h
)

target_link_libraries(xz-stream-test
    -llzma
)

# copy script.sh to build-dir, just for test only
add_custom_target(dummy ALL
    COMMAND echo "This is ALL target 'zoo', and it depends on ${TEST_FILE}"
    DEPENDS
        script.sh
    VERBATIM
)
add_custom_command(OUTPUT script.sh
    COMMENT
        "creating script.sh"
    COMMAND
        cp ../../script.sh ..
)


# Simple sqlite3 compile test; run with ctest
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
