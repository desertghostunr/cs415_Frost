#!/bin/bash
BUILD_DIR="build/"

# building setup
if [ ! -d "$BUILD_DIR" ]; then
	mkdir build
fi
cd build
cp ../makefile .

# building
make

echo "PA1 timing test" && make run_timing_test

# cleaning (optional for build script)
# make clean
