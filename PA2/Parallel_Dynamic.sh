#!/bin/bash
#SBATCH -n 8
#SBATCH --mem=2048
#SBATCH --time=00:10:00

srun PA2_Parallel_Dynamic 512 512 parallel_d.pgm
