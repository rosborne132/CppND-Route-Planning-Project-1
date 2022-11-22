#!/bin/bash

echo -n "Running process $1"

cd scripts

case $1 in
  start)
    ./start.sh
    ;;

  test)
    ./test.sh
    ;;

  *)
    echo -n "unknown"
    ;;
esac

echo "Process complete!"