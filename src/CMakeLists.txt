cmake_minimum_required(VERSION 3.12)

project(dxtionary-db)
# TODO: remve these two commands after test
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add the executable
## dxtionary-db
add_executable(dxtionary-db
    dxtionary_db.cpp
)
target_include_directories(dxtionary-db PUBLIC
    "${PROJECT_BINARY_DIR}"
)
target_link_libraries(dxtionary-db
    PRIVATE
        dxtionary_bind
)

## dxtionary_bind
add_library(dxtionary_bind
    STATIC
        dxtionary_bind.cpp
        dxtionary_bind.h
)
target_link_libraries(dxtionary_bind
    PUBLIC
        sqlite3 -lm
)
target_include_directories(dxtionary_bind
    INTERFACE
        ${PROJECT_SOURCE_DIR}
)

## import-raw-dict
add_executable(import-raw-dict
    import_raw_dict.cpp
)
target_link_libraries(import-raw-dict
    PRIVATE
        gz_stream
        dict_file_processor
)
### gz_stream
add_library(gz_stream
    STATIC
        gz_stream.cpp
        gz_stream.hpp
)
target_link_libraries(gz_stream
    PUBLIC
        zlibstatic
)
target_include_directories(gz_stream
    INTERFACE
        ${PROJECT_SOURCE_DIR}
    PUBLIC
        ${PROJECT_BINARY_DIR}/../ext/zlib
)

### dict_file_processor
add_library(dict_file_processor
    STATIC
        dict_file_processor.cpp
        dict_file_processor.hpp
)
target_compile_features(dict_file_processor
    PUBLIC
        cxx_std_17
)
target_link_libraries(dict_file_processor
    PUBLIC
        -lstdc++fs
        gz_stream
        sqlite3 -lm
)
target_include_directories(dict_file_processor
    INTERFACE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_BINARY_DIR}
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

do_test(editdist3 dxtionary-db "SELECT editdist3('Julia', 'Jullian') as dist;" "dist = 200")
do_test(json_quote dxtionary-db "SELECT json_quote(json_object('name', 'Jull\nian')) as json;" "json =.*")
# do_test(sql_error dxtionary-db "CREATE TABLE IF NOT EXIST wiktionary(a TEXT, b TEXT, c TEXT); INSERT INTO wiktionary VALUES('D', 'DD', 'Das Buchstaben D', 'X')" "SQL error: table wiktionary has 3 columns but 4 values were supplied")
