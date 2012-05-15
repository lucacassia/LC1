#!/bin/bash

for (( i=1000; i<=65392000; i*=2 ))
do
  echo "loop $i"
  ./metropolis2 $i
  echo
done

