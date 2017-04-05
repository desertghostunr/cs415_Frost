#!/bin/bash

n=$1

if ! [[ -n "$n" ]]; then
	n="100000000"
fi

#sequential
for (( j = 0; j < 5; j++ ))
do
	for (( i=10; i<=$n;i=10*i ))
	do
		echo "Running test of $i numbers"
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

		while [[ "$TEST" =~ "andrewfrost" ]]
		do
			sleep 5s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch Sequential.sh "$i" "100"

		sleep 5s
	done
done

#parallel
for (( j = 0; j < 5; j++ ))
do
	for (( i=10; i<=$n;i=10*i ))
	do
		for (( k=2; k<=32;k=2*k ))
		do
			echo "Running test of $i numbers and $k tasks on $(( (($k - 1) / 8) + 1 )) boxes"

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [[ "$TEST" =~ "andrewfrost" ]]
			do
				sleep 5s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$k" Parallel.sh "$i"

			sleep 5s

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [[ "$TEST" =~ "andrewfrost" ]]
			do
				sleep 5s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$(($k+1))" Parallel.sh "$i"

		done
		sleep 2s

		echo "Running test of $i numbers and 24 tasks"

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [[ "$TEST" =~ "andrewfrost " ]]
		do
			sleep 5s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "24" Parallel.sh "$i"

		sleep 2s

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [[ "$TEST" =~ "andrewfrost" ]]
		do
				sleep 5s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "25" Parallel.sh "$i"
		sleep 5s
	done

	sleep 5s

done

#sequential tests
for (( j=0; j<5; j++ ))
do
	for (( i=500000; i<=1000000000;i=i+500000 ))
	do
		echo "Running test of $i numbers"
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [[ "$TEST" =~ "andrewfrost" ]]
		do
			sleep 5s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch Sequential.sh "$i" "100"

		sleep 5s
	done

	sleep 5s
done

#parallel tests
for (( j = 0; j < 5; j++ ))
do
	for (( i=500000; i<=1000000000;i=i+500000 ))
	do
		for (( k=2; k<=32;k=2*k ))
		do
			echo "Running test of $i numbers and $k tasks on $(( (($k - 1) / 8) + 1 )) boxes"

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [[ "$TEST" =~ "andrewfrost" ]]
			do
				sleep 5s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$k" Parallel.sh "$i"

			sleep 5s

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [[ "$TEST" =~ "andrewfrost" ]]
			do
				sleep 2s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$(($k+1))" Parallel.sh "$i"

		done
		sleep 5s

		echo "Running test of $i numbers and 24 tasks"

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [[ "$TEST" =~ "andrewfrost" ]]
		do
			sleep 5s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "24" Parallel.sh "$i"

		sleep 5s

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [[ "$TEST" =~ "andrewfrost" ]]
		do
				sleep 5s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "25" Parallel.sh "$i"
		sleep 5s
	done
	sleep 5s
done

