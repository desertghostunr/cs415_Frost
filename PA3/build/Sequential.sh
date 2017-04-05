#!/bin/bash

file=$1
buckets=$2
save=$3

if ! [[ -n "$file" ]]; then
	file="1000"
fi

if ! [[ -n "$buckets" ]]; then
	buckets="-1"
fi

if ! [[ -n "$save" ]]; then
	save="0"
fi

#SBATCH -n 1
#SBATCH --mem=16384
#SBATCH --time=00:20:00
#SBATCH -o ../bin/sequential-%j.out

./Sequential "$file" "$buckets" "$save"
