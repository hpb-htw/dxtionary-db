#!/bin/bash

# NOTE: not work at all, see later if have time

platform="i686-w64-mingw32"
# alternatives:
# x86_64-w64-mingw32
# i686-w64-mingw32
# i586-mingw32msvc
# x86_64-w64-mingw32

# C
export CC=$(which "${platform}-gcc")

# C++
export CXX=$(which "${platform}-g++")

# setup windres
WINRES=$(which "${platform}-windres")


cmake -S . -B cmake-build-mingw -DSKIP_INSTALL_HEADERS=False -DCMAKE_RC_COMPILER=${WINRES}