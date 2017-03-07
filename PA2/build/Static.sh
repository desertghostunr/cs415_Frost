#!/bin/bash

width=$1
height=$2

if ![[ -n "$width" ]]; then
	width="512"
fi

if ! [[ -n "$height" ]]; then
	height="512"
fi

#SBATCH -n 3
#SBATCH --mem=2048
#SBATCH --time=00:05:00

srun PA2_Parallel_Static "$width" "$height" images/parallel_s_"$width"x"$height".pgm

