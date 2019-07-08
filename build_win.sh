#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build
mkdir -p $BUILD
cd $BUILD
cmake -G "MSYS Makefiles" -DCMAKE_PREFIX_PATH=C:/SDK/Qt5.12.3/5.12.3/mingw73_64/lib/cmake -DCMAKE_BUILD_TYPE=MinSizeRel ../ || exit 1
make -j4
# make install
