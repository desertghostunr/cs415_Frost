#!/bin/bash

nodes=$1
width=$2
height=$3

if [[ -n "$nodes" ]]; then
	#do nothing
else
	nodes="3"
fi

if [[ -n "$width" ]]; then
	#do nothing
else
	width="512"
fi

if [[ -n "$height" ]]; then
	#do nothing
else
	height="512"
fi

#SBATCH -n "$nodes"
#SBATCH --mem=2048
#SBATCH --time=00:05:00

srun PA2_Parallel_Dynamic "$width" "$height" images/parallel_d_"$width"x"$height".pgm

