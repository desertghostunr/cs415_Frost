#!/bin/bash

file=$1
save=$2

if ! [[ -n "$file" ]]; then
	file="../bin/data.txt"
fi

if ! [[ -n "$save" ]]; then
	save="-1"
fi

#SBATCH -n 2
#SBATCH --mem=2048
#SBATCH --time=00:05:00
#SBATCH -o ../bin/parallel-%j.out

srun Parallel "$file" "$save"
