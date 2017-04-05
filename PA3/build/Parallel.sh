#!/bin/bash

file=$1
save=$2

if ! [[ -n "$file" ]]; then
	file="1000"
fi

if ! [[ -n "$save" ]]; then
	save="-1"
fi

#SBATCH -n 2
#SBATCH --mem=16384
#SBATCH --time=00:06:00
#SBATCH -o ../bin/parallel-%j.out

srun Parallel "$file" "$save"
