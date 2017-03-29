#!/bin/bash

n=$1

if ! [[ -n "$n" ]]; then
	n="100000000"
fi

for (( i=10; i<=$n;i=5*i ))
do
	echo "Generating $i numbers"

	srun n1 generator "$i" > ../bin/data"$i".txt
done

for (( j = 0; j < 5; j++ ))

	for (( i=10; i<=$n;i=5*i ))
	do
		echo "Running test of $i numbers"

		sbatch Sequential.sh ../bin/data"$i".txt

		sleep 2.5
	done
	
	sleep 60
	
done
