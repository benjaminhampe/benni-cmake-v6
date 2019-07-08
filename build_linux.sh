#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build
mkdir -p $BUILD
cd $BUILD
cmake -DCMAKE_BUILD_TYPE=MinSizeRel ../ || exit 1
make -j8
# make install
