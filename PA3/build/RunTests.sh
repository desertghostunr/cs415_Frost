#!/bin/bash

n=$1

if ! [[ -n "$n" ]]; then
	n="100000000"
fi

#sequential
for (( j = 0; j < 4; j++ ))
do
	for (( i=10; i<=$n;i=5*i ))
	do
		for (( k=2; k<=32;k=2*k ))
		do
			echo "Running test of $i numbers"

			sbatch Sequential.sh ../bin/data"$i".txt "$k"

			sleep 30
			squeue
		done
		
		sleep 60
		sbatch Sequential.sh ../bin/data"$i".txt
		squeue
	done

	sleep 60

done

#parallel
for (( j = 0; j < 5; j++ ))
do
	for (( i=10; i<=$n;i=5*i ))
	do
		for (( k=2; k<=32;k=2*k ))
		do
			echo "Running test of $i numbers and $k tasks on $(( (($k - 1) / 8) + 1 )) boxes"

			sbatch -N "$(( (($k - 1) / 8) + 1 ))" -n "$k" Parallel.sh ../bin/data"$i".txt

			sleep 25
			squeue
		done
		sleep 35
	done

	sleep 120

done

echo "Pausing before running gradual tests"
sleep 180


#sequential tests
for (( j=0; j<5; j++ ))
do
	for (( i=400000; i<=$n;i+=4980000 ))
	do
		for (( k=2; k<=32;k=2*k ))
		do
			echo "Running test of $i numbers"

			sbatch Sequential.sh ../bin/data"$i".txt "$k"

			sleep 30
			squeue
		done
		
		sleep 60
		sbatch Sequential.sh ../bin/data"$i".txt
		squeue
	done

	sleep 120
done

#parallel tests
for (( j = 0; j < 5; j++ ))
do
	for (( i=400000; i<=$n;i+=4980000 ))
	do
		for (( k=2; k<=32;k=2*k ))
		do
			echo "Running test of $i numbers and $k tasks on $(( (($k - 1) / 8) + 1 )) boxes"

			sbatch -N "$(( (($k - 1) / 8) + 1 ))" -n "$k" Parallel.sh ../bin/data"$i".txt

			sleep 20
			squeue
		done
		sleep 45
	done
	sleep 120
done

