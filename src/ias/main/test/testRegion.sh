#!/bin/bash


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


pushd $SCRIPT_DIR > /dev/null


IAS_APP=./../iascli
DATA_DIR=../../test/data


echo -e "Testing calling find_regions argument (window should be presented)"
$IAS_APP --logcout --image=$DATA_DIR/test1.png --findRegion=200,200,0,0,249,20 --displayJoin --savePixels=out1.png
EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
	echo "Test failed -- could not find regions"
	exit 1
else
	echo "Passed"
fi


echo -e "\nTesting calling find_regions argument (window should be presented)"
$IAS_APP --logcout --image=$DATA_DIR/test1.png --findRegion=0,0,255,255,255,20 --displayJoin --savePixels=out1b.png
EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
	echo "Test failed -- could not find regions"
	exit 1
else
	echo "Passed"
fi


popd > /dev/null
