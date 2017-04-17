#!/bin/bash

file1=$1
file2=$2
save=$3

if ! [[ -n "$file1" ]]; then
	file1="60"
fi

if ! [[ -n "$file2" ]]; then
	file2="0"
fi

if ! [[ -n "$save" ]]; then
	save="-1"
fi

#SBATCH -n 4
#SBATCH --mem=8192
#SBATCH --time=00:08:00
#SBATCH -o ../bin/parallel-%j.out

srun Parallel "$file1" "$file2" "$save"
