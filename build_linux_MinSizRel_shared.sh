#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/linux64_MinSizRel_shared
mkdir -p $BUILD
cd $BUILD
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DBENNI_SDK_SHARED=1 ../../ || exit 1
make -j8
# make install
