#!/bin/bash

############################################
#    script to build with clang on linux   #
############################################

export CC=`which cc`
export CXX=`which c++`

cmake -S ./ -B gcc-build