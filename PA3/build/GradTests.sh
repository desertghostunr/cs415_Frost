#!/bin/bash

n=$1

if ! [[ -n "$n" ]]; then
	n="100000000"
fi

#for (( i=400000; i<=$n;i+=4980000 ))
#do
#	echo "Generating $i numbers"

#	./generator "$i" > ../bin/data"$i".txt
#done


for (( i=400000; i<=$n;i+=4980000 ))
do
	echo "Running test of $i numbers"

	sbatch Sequential.sh ../bin/data"$i".txt

	sleep 8
	squeue
done
