#!/bin/bash

file=$1
buckets=$2
save=$3

if ! [[ -n "$file" ]]; then
	file="../bin/data.txt"
fi

if ! [[ -n "$buckets" ]]; then
	buckets="-1"
fi

if ! [[ -n "$save" ]]; then
	save="0"
fi

#SBATCH -n 1
#SBATCH --mem=2048
#SBATCH --time=00:05:00
#SBATCH -o ../bin/sequential-%j.out

./Sequential "$file" "$buckets" "$save"
