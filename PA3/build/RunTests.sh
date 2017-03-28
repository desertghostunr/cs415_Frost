#!/bin/bash

n=$1

if ! [[ -n "$n" ]]; then
	n="1000000"
fi

for (( i=0; i<=$n;i+=100 )) 
do
	./generator < $i > ../bin/data"$i".txt
done


for (( i=0; i<=$n;i+=100 )) 
do
	echo "Running test $i"

	sbatch Sequential.sh ../bin/data"$i".txt

	sleep 1m
done
