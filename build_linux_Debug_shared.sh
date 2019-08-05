#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/linux64_Debug_shared
mkdir -p $BUILD
cd $BUILD
cmake -DCMAKE_BUILD_TYPE=Debug -DBENNI_SDK_SHARED=1 ../../ || exit 1
make -j3
# make install
