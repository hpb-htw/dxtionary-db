set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

# Setup for profiling
#
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(COVERAGE_COMPILER_FLAGS "-g -O0 -fprofile-instr-generate -fcoverage-mapping"
        CACHE INTERNAL "")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

set(TEST_BIN "") # empty list
set(OBJECT_FILE "") # empty list
set(TEST_SRC "") # empty list

function(llvm_cover TARGET_NAME)
    add_custom_target(${TARGET_NAME}-ccov-preprocessing
        COMMAND LLVM_PROFILE_FILE=${TARGET_NAME}.profraw $<TARGET_FILE:${TARGET_NAME}>
        COMMAND llvm-profdata merge -sparse ${TARGET_NAME}.profraw -o ${TARGET_NAME}.profdata
        DEPENDS ${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_target(${TARGET_NAME}-ccov-show
        COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata -show-line-counts-or-regions ${ARGN}
        DEPENDS ${TARGET_NAME}-ccov-preprocessing
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_target(${TARGET_NAME}-ccov-report
        COMMAND llvm-cov report $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata ${ARGN}
        DEPENDS ${TARGET_NAME}-ccov-preprocessing
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_target(${TARGET_NAME}-ccov
        COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata -show-line-counts-or-regions -output-dir=${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-llvm-cov -format="html" ${ARGN}
        DEPENDS ${TARGET_NAME}-ccov-preprocessing
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    # append new bin file to TEST_BIN
    set(TEST_BIN ${TEST_BIN} ${TARGET_NAME} PARENT_SCOPE)
    # append new -object ${TARGET_NAME}
    set(LOCAL_OBJ_FILE ${OBJECT_FILE} -object)
    set(OBJECT_FILE ${LOCAL_OBJ_FILE} ${TARGET_NAME} PARENT_SCOPE)
    # append new source file to TEST_SRC
    set(TEST_SRC ${TEST_SRC} ${ARGN} PARENT_SCOPE)
endfunction()

function(llvm_cover_report_all)
    message(STATUS "TEST_BIN ${TEST_BIN}")

    set(PROF_RAW ${TEST_BIN})
    list(TRANSFORM PROF_RAW APPEND ".profraw")
    message(STATUS "PROF_RAW ${PROF_RAW}")

    message(STATUS "OBJECT_FILE ${OBJECT_FILE}")
    message(STATUS "TEST_SRC ${TEST_SRC}")

    add_custom_target(llvm_cover_report_all
        COMMAND llvm-profdata merge ${PROF_RAW} -output=llvm_cover_report_all.profdata
        COMMAND llvm-cov report ${TEST_BIN}                            -instr-profile=llvm_cover_report_all.profdata ${OBJECT_FILE} ${TEST_SRC}
        COMMAND llvm-cov export ${TEST_BIN} -format=lcov               -instr-profile=llvm_cover_report_all.profdata ${OBJECT_FILE} ${TEST_SRC} > llvm_cover_report_all.lcov
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    foreach(profraw IN LISTS TEST_BIN)
        add_dependencies(llvm_cover_report_all ${profraw}-ccov-preprocessing)
    endforeach()
endfunction()