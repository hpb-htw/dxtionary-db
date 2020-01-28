cmake_minimum_required(VERSION 3.12)

project(dxtionary-db)

# Enable testing for the project
enable_testing()

add_executable(gz_stream_test gz_stream_test.cpp)
target_link_libraries(gz_stream_test
    PRIVATE
        gtest_main
        gz_stream
)
target_include_directories(gz_stream_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
add_test(NAME gz_stream_test COMMAND gz_stream_test)