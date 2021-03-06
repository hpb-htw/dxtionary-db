cmake_minimum_required(VERSION 3.12)

project(dxtionary-db)
# TODO: remve these two commands after test
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}")
set(BIN_DIR ${PROJECT_SOURCE_DIR}/../bin/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})

set(CMAKE_VERBOSE_MAKEFILE on)

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
target_compile_options(dict_file_processor
    PUBLIC
    -std=c++17
)
target_compile_features(dict_file_processor
    PUBLIC
        cxx_std_17
)

target_include_directories(dict_file_processor
    INTERFACE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_BINARY_DIR}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(dict_file_processor
        PUBLIC
            -lstdc++fs # MacOS doesn't like this
            -lm
            gz_stream
            sqlite3
    )
else()
    target_link_libraries(dict_file_processor
        PUBLIC
            -lm
            gz_stream
            sqlite3
    )
endif()

function(copy_executable_file_to_bin EXE_TARGET)
    foreach(exe_file IN LISTS ARGV)
        add_custom_command(TARGET ${exe_file}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${exe_file}> ${BIN_DIR}/$<TARGET_FILE_NAME:${exe_file}>
        )
    endforeach()
endfunction()

# copy executable files to BIN_DIR
copy_executable_file_to_bin(
    dxtionary-db import-raw-dict
)

