#!/bin/bash

width=$1
height=$2

if ![[ -n "$width" ]]; then
	width="512"
fi

if ! [[ -n "$height" ]]; then
	height="512"
fi

#SBATCH -n 1
#SBATCH --mem=2048
#SBATCH --time=00:05:00

./PA2_Sequential "$width" "$height" images/sequential_"$width"x"$height".pgm
