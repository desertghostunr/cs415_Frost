#!/bin/bash
#SBATCH -n 5
#SBATCH --mem=2048
#SBATCH --time=00:10:00

srun PA2_Parallel_Dynamic 8192 8192 images/parallel_d.pgm
