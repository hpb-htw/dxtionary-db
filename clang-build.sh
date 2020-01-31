#!/bin/bash

############################################
#    script to build with clang on linux   #
############################################

#export CC=`which clang`
#export CXX=`which clang++`

#cmake -S ./ -B clang-build
cmake -S . -B clang-build -DCMAKE_TOOLCHAIN_FILE=clang-toolchain.cmake