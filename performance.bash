#!/bin/bash

RADIUS=0.13

lod=2
rm out.txt
while [[ ${lod} -ne 6 ]]; do
  n_spheres=1
  while [[ ${n_spheres} -ne 101 ]]; do
    echo ${n_spheres} > "in.txt"
    echo ${RADIUS} >> "in.txt"
    echo ${lod} >> "in.txt"
    echo 100 >> "in.txt"

    time=$(./build/advanced_computer_graphics < "in.txt")
    time=${time#*"Avg. display time: "}
    time=${time%"ms."}

    echo "${n_spheres} ${lod} ${time}" >> out.txt

    n_spheres=$(( ${n_spheres} + 1 ))
  done
  lod=$(( ${lod} + 1 ))
done
rm "in.txt"
