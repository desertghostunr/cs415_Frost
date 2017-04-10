#!/bin/bash

#sequential
for (( i=12; i<=192;i=2*i ))
do
	echo "Running test of $i numbers"
	TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

	while [[ "$TEST" =~ "andrewfrost" ]]
	do
		sleep 5s
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
	done

	sbatch Sequential.sh "$i"

	sleep 5s
done

for (( i=384; i<=2688;i=i+192 ))
do
	echo "Running test of $i numbers"
	TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

	while [[ "$TEST" =~ "andrewfrost" ]]
	do
		sleep 5s
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
	done

	sbatch Sequential.sh "$i"

	sleep 5s
done
