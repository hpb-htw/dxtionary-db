#!/bin/bash

############################################
#    script to build with clang on linux   #
############################################

cmake -S . -B clang-build -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake