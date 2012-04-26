#!/bin/bash

for run in {1..100}
do
  echo "loop $run"
  ./metropolis3
done

