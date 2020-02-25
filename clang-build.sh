#!/bin/bash

############################################
#    script to build with clang on linux   #
############################################

# ---------------------------------------- #

cmake -S . -B clang-build       -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cd clang-build && cmake --build . && cd test && ctest && cd ../..
# to make debug version:
cmake -S . -B clang-build-debug -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cd clang-build-debug && cmake --build . && cd test && ctest && cd ../..