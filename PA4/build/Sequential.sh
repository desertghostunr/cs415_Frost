#!/bin/bash

file=$1
save=$2

if ! [[ -n "$file" ]]; then
	file="1000"
fi

if ! [[ -n "$save" ]]; then
	save="0"
fi

#SBATCH -n 1
#SBATCH --mem=8192
#SBATCH --time=00:08:00
#SBATCH -o ../bin/sequential-%j.out

./Sequential "$file" "$save"
