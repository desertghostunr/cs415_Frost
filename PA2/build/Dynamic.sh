#!/bin/bash

nodes=$1
width=$2
height=$3

if ! [[ -n "$nodes" ]]; then
	nodes="3"
fi

if ![[ -n "$width" ]]; then
	width="512"
fi

if ! [[ -n "$height" ]]; then
	height="512"
fi

#SBATCH -n "$nodes"
#SBATCH --mem=2048
#SBATCH --time=00:05:00

srun PA2_Parallel_Dynamic "$width" "$height" images/parallel_d_"$width"x"$height".pgm

