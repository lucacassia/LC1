#!/bin/bash

for (( i=10; i<=11000; i+=10 ))
do
  echo "loop $i"
  ./pi $i
  echo
done

