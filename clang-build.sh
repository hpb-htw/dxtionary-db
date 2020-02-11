#!/bin/bash

############################################
#    script to build with clang on linux   #
############################################

# ---------------------------------------- #

cmake -S . -B clang-build -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
# to make debug version:
cmake -S . -B clang-build-debug -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake -DCMAKE_BUILD_TYPE=Debug