#!/bin/bash

echo "Starting process"

./build.sh

echo "Starting tests!"

cd ../build

./test
