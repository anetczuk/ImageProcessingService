#!/bin/bash


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


BUILD_DIR=$SCRIPT_DIR/../build/DEBUG_CLANG

mkdir -p $BUILD_DIR


cd $BUILD_DIR

cmake $SCRIPT_DIR -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug
