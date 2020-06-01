#!/bin/bash

# Exit when any command fails.
set -e

mkdir -p build
cd build
cmake ..
make ecosim
cd ..
./build/ecosim
