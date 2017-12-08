#!/bin/bash


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


BUILD_DIR=$SCRIPT_DIR/../build/DEBUG_GCC

mkdir -p $BUILD_DIR


cd $BUILD_DIR

cmake $SCRIPT_DIR -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug
