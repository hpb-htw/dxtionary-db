#!/bin/bash

# C
# export CC=`which i686-w64-mingw32-gcc`
export CC=`which x86_64-w64-mingw32-gcc`

# C++
# export CXX=`which i686-w64-mingw32-g++`
export CXX=`which x86_64-w64-mingw32-g++`

cmake -S . -B cmake-build-mingw -DSKIP_INSTALL_HEADERS=False