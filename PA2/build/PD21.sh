#!/bin/bash
#SBATCH -n 21
#SBATCH --mem=2048
#SBATCH --time=00:10:00

srun PA2_Parallel_Dynamic 16384 16384 images/parallel_dd.pgm

