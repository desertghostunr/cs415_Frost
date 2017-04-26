#!/bin/bash

#parallel
for (( i=60; i<=2640;i=i+60 ))
do
	echo "Running test of $i numbers"
	TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

	while [[ "$TEST" =~ "andrewfrost" ]]
	do
		sleep 3s
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
	done

	sbatch -n 4  Parallel.sh "$i"

	sleep 3s

	echo "Running test of $i numbers"
        TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

        while [[ "$TEST" =~ "andrewfrost" ]]
        do
                sleep 3s
                TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
        done

        sbatch -n 9  Parallel.sh "$i"

        sleep 3s

        echo "Running test of $i numbers"
        TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

        while [[ "$TEST" =~ "andrewfrost" ]]
        do
                sleep 3s
                TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
        done

        sbatch -n 16 Parallel.sh "$i"

        sleep 3s

	echo "Running test of $i numbers"
        TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

        while [[ "$TEST" =~ "andrewfrost" ]]
        do
                sleep 3s
                TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
        done

        sbatch -n 25 Parallel.sh "$i"

        sleep 3s


done

for (( i=12; i<=60;i=i+12 ))
do
	echo "Running test of $i numbers"
	TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

	while [[ "$TEST" =~ "andrewfrost" ]]
	do
		sleep 2s
		TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
	done

	sbatch -n 4 Parallel.sh "$i"

	sleep 2s

	echo "Running test of $i numbers"
        TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

        while [[ "$TEST" =~ "andrewfrost" ]]
        do
                sleep 2s
                TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
        done

        sbatch -n 9 Parallel.sh "$i"

        sleep 2s

	echo "Running test of $i numbers"
        TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

        while [[ "$TEST" =~ "andrewfrost" ]]
        do
                sleep 2s
                TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
        done

        sbatch -n 16 Parallel.sh "$i"

        sleep 2s


done

for (( i=5; i<=60;i=i+5 ))
do
        echo "Running test of $i numbers"
        TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")

        while [[ "$TEST" =~ "andrewfrost" ]]
        do
                sleep 2s
                TEST=$(squeue -o"%.18i %.9P %.20j %.20u %.8T %.10M %.9l %.6D %R")
        done

        sbatch -n 25 Parallel.sh "$i"

        sleep 2s
done

