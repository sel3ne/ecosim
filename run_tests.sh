#!/bin/bash

# Exit when any command fails.
set -e

mkdir -p build
cd build
cmake ..
make -j$(nproc) ecosim_tests
cd ..
./build/ecosim_tests
