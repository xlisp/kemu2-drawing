#!/bin/bash

rm -fr build

mkdir build
cd build

# Generate build files and compile
cmake ..
make

./driving_game

