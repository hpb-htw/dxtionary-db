cmake_minimum_required(VERSION 3.12)

project(dxtionary-db)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
    )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
    )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#[==[ Add googletest directly to our build. This adds the following targets:
      gtest, gtest_main, gmock and gmock_main
#]==]
add_subdirectory(
    "${CMAKE_BINARY_DIR}/googletest-src"
    "${CMAKE_BINARY_DIR}/googletest-build"
    EXCLUDE_FROM_ALL
)

#[==[ The gtest/gmock targets carry header search path dependencies
      automatically when using CMake 2.8.11 or later. Otherwise we
      have to add them here ourselves.
#]==]
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
        "${gmock_SOURCE_DIR}/include"
        )
endif()





# Enable testing for the project
enable_testing()
include(GoogleTest)
message(STATUS ${PROJECT_BINARY_DIR})
configure_file(${PROJECT_SOURCE_DIR}/data.txt.gz ${PROJECT_BINARY_DIR}/data.txt.gz COPYONLY)

## gz_stream_test
add_executable(gz_stream_test gz_stream_test.cpp)
target_link_libraries(gz_stream_test
    PRIVATE
        gtest_main
        gz_stream
)

gtest_add_tests(
    TARGET gz_stream_test
    TEST_LIST gz_test
)

## dict_file_processor_test
add_executable(dict_file_processor_test dict_file_processor_test.cpp)
target_link_libraries(dict_file_processor_test
    PRIVATE
        gtest_main
        dict_file_processor
)
gtest_add_tests(
    TARGET dict_file_processor_test
    TEST_LIST gz_test
)



# Code coverage configuration only for g++ compiler
if(CMAKE_COMPILER_IS_GNUCXX)
    ## dummy target to hook Post build
    add_custom_target(
        run_test ALL
        DEPENDS
            gz_stream_test
            dict_file_processor_test
    )
    ## call ctest to run all unit test
    add_custom_command(TARGET run_test
        POST_BUILD
        COMMAND ctest --output-on-failure
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    ## TODO: call gcov to make coverage report
endif()