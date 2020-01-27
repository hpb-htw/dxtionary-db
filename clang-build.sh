#!/bin/bash

############################################
#    script to build with clang on linux   #
############################################

export CC=`which clang`
export CXX=`which clang++`

cmake -S ./ -B cmake-build-clang