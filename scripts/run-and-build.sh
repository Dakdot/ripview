#!/bin/bash
EXECUTABLE_NAME=ripview

if [[ "$OSTYPE" == "darwin"* ]]; then
    NUM_PROC="$(sysctl -n hw.logicalcpu)"
else
    NUM_PROC="$(nproc)"
fi

mkdir -p build

cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug ..
if [[ $? -ne 0 ]]; then
    echo CMake build stage failed.
    exit $?
fi

make -j $NUM_PROC
if [[ $? -ne 0 ]]; then
    echo Make build stage failed.
    exit $?
fi

clear
if  command -v mangohud >/dev/null 2>&1; then
    mangohud ./$EXECUTABLE_NAME
else
    ./$EXECUTABLE_NAME
fi
