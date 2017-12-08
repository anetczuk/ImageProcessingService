#!/bin/bash


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


pushd $SCRIPT_DIR > /dev/null

./ias_test "$@"


popd > /dev/null
