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
	save="0"
fi

#SBATCH -n 1
#SBATCH --mem=2048
#SBATCH --time=00:07:00
#SBATCH -o ../bin/sequential-%j.out

./Sequential "$file1" "$file2" "$save"
