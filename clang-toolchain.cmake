set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

# Setup for profiling
#
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(COVERAGE_COMPILER_FLAGS "-g -fprofile-instr-generate -fcoverage-mapping"
        CACHE INTERNAL "")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)



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
endfunction()