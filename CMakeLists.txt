cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(dxtionary-db VERSION 1.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)




configure_file(dxtionary_version.in.h dxtionary_version.h)

# add the sqlite3 library
add_subdirectory(sqlite3)
list(APPEND EXTRA_LIBS sqlite3)




# add the executable
add_executable(dxtionary-db dxtionary-db.cpp)
target_link_libraries(dxtionary-db PUBLIC
        ${EXTRA_LIBS} -ldl -lm
)


target_include_directories(dxtionary-db PUBLIC
    "${PROJECT_BINARY_DIR}"
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
    add_test(NAME Comp${name}
        COMMAND ${target} "/tmp/test.db" ${select}
    )
    message(select)
    set_tests_properties( Comp${name}
        PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

do_test(editdist3 dxtionary-db "SELECT editdist3(\"Julia\", \"Jullian\") as dist;" "dist = 200")
do_test(json_quote dxtionary-db "SELECT json_quote(json_object(\"name\", \"Jull\nian\")) as json;" "json =.*")
# do_test(sql_error dxtionary-db "CREATE TABLE IF NOT EXIST wiktionary(a TEXT, b TEXT, c TEXT); INSERT INTO wiktionary VALUES('D', 'DD', 'Das Buchstaben D', 'X')" "SQL error: table wiktionary has 3 columns but 4 values were supplied")
