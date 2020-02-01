#!/bin/bash

###################################################
#    script to build with gcc and gcov on linux   #
###################################################


cmake -S ./ -B gcc-build  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=gcov-toolchain.cmake

# To make reports
# cd gcc-build/CMakeFile/<target>.dir
# gcov <file>.<extension>.gcnp
# lcov --capture --directory . --output-file capture.info
# genhtml capture.info

