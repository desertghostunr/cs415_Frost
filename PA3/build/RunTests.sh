#!/bin/bash

n=$1

if ! [[ -n "$n" ]]; then
	n="1000000000"
fi

#sequential
for (( j = 0; j < 4; j++ ))
do
	for (( i=10; i<=$n;i=10*i ))
	do
		echo "Running test of $i numbers"
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [["$TEST"=~"andrewfr"]]
		do
			sleep 2s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done		
		
		sbatch Sequential.sh "$i"

		sleep 2s
	done
done

#parallel
for (( j = 0; j < 5; j++ ))
do
	for (( i=10; i<=$n;i=10*i ))
	do
		for (( k=2; k<=16;k=2*k ))
		do
			echo "Running test of $i numbers and $k tasks on $(( (($k - 1) / 8) + 1 )) boxes"

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [["$TEST"=~"andrewfr"]]
			do
				sleep 2s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$k" Parallel.sh "$i"

			sleep 2s

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [["$TEST"=~"andrewfr"]]
			do
				sleep 2s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$(($k+1))" Parallel.sh "$i"
			
		done
		sleep 2s

		echo "Running test of $i numbers and 24 tasks"

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [["$TEST"=~"andrewfr"]]
		do
			sleep 2s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "24" Parallel.sh "$i"

		sleep 2s

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [["$TEST"=~"andrewfr"]]
		do
				sleep 2s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "25" Parallel.sh "$i"
		sleep 2s
	done

	sleep 2s

done

#sequential tests
for (( j=0; j<5; j++ ))
do
	for (( i=200000; i<=2000000000;i=10*i))
	do
		echo "Running test of $i numbers"
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [["$TEST"=~"andrewfr"]]
		do
			sleep 2s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done		
		
		sbatch Sequential.sh "$i"

		sleep 2s
	done

	sleep 2s
done

#parallel tests
for (( j = 0; j < 5; j++ ))
do
	for (( i=200000; i<=2000000000;i=10*i))
	do
		for (( k=2; k<=16;k=2*k ))
		do
			echo "Running test of $i numbers and $k tasks on $(( (($k - 1) / 8) + 1 )) boxes"

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [["$TEST"=~"andrewfr"]]
			do
				sleep 2s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$k" Parallel.sh "$i"

			sleep 2s

			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			while [["$TEST"=~"andrewfr"]]
			do
				sleep 2s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
			done

			sbatch -n "$(($k+1))" Parallel.sh "$i"
			
		done
		sleep 2s

		echo "Running test of $i numbers and 24 tasks"

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [["$TEST"=~"andrewfr"]]
		do
			sleep 2s
			TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "24" Parallel.sh "$i"

		sleep 2s

		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		while [["$TEST"=~"andrewfr"]]
		do
				sleep 2s
				TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
		done

		sbatch -n "25" Parallel.sh "$i"
		sleep 2s
	done
	sleep 2s
done

