#!/bin/bash


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


pushd $SCRIPT_DIR > /dev/null


IAS_APP=./../iascli
DATA_DIR=../../test/data


echo -e "Testing running application without parameters"
$IAS_APP
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
	echo "Test failed -- should return error message"
	exit 1
else
	echo "Passed"
fi


echo -e "\nTesting passing help parameter"
$IAS_APP --help
EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
	echo "Test failed"
	exit 1
else
	echo "Passed"
fi


echo -e "\nTesting loading non-existing image"
$IAS_APP --image=$DATA_DIR/non_existing_file.png
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
	echo "Test failed -- should return non-zero exit code"
	exit 1
else
	echo "Passed"
fi


echo -e "\nTesting loading image"
$IAS_APP --image=$DATA_DIR/test1.png
EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
	echo "Test failed -- could not load image"
	exit 1
else
	echo "Passed"
fi


echo -e "\nTesting displaying image (window should be presented)"
$IAS_APP --image=$DATA_DIR/test1.png --displayImage
EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
	echo "Test failed -- could not display image"
	exit 1
else
	echo "Passed"
fi


popd > /dev/null
