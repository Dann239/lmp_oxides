#!/bin/bash

scale_start=$1
scale_end=$2
scale_step=$3

while [[ `./help2 $scale_end $scale_start` -eq 1 ]]
do
    scripts/measure.sh $scale_start
    scale_start=$(./help $scale_start $scale_step) 
done
