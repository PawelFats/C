#!/bin/bash

for ((i=500; i<=10000; i+=500)); do
    ./bench --memory-type RAM --block-size ${i}Mb --launch-count 10
    echo "Test number: $i";
done
