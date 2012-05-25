#!/bin/bash

for (( i=10; i<=1100; i+=10 ))
do
  echo "loop $i"
  ./integral $i
  echo
done

