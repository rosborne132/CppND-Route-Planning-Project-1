#!/bin/bash

echo "Starting process"

./build.sh

echo "Starting app!"

cd ../build

./OSM_A_star_search -f ../map.osm
