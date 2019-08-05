#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/linux64_Debug_static
mkdir -p $BUILD
cd $BUILD
cmake -DCMAKE_BUILD_TYPE=Debug ../../ || exit 1
make -j8
# make install
