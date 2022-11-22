#!/bin/bash

echo "Starting process"

cd ..

if [ ! -f "build" ]; then
    echo "Generating Makefile" && mkdir build
fi

cd build

# Check if Makefile exists. If not then create Makefile.
if [ ! -f "Makefile" ]; then
    echo "Generating Makefile" && cmake ..
fi

echo "Building project"

make

echo "Build complete! :)"
