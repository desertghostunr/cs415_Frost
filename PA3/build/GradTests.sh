#!/bin/bash

n=$1

if ! [[ -n "$n" ]]; then
	n="100000000"
fi

for (( i=400000; i<=$n;i+=4980000 ))
do
	echo "Generating $i numbers"

	srun n1 generator "$i" > ../bin/data"$i".txt
done

for (( j=0; j<5; j++ ))

	for (( i=400000; i<=$n;i+=4980000 ))
	do
		echo "Running test of $i numbers"

		sbatch Sequential.sh ../bin/data"$i".txt

		sleep 20
		squeue
	done
	
	sleep 120
done
