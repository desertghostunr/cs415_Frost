#!/bin/bash
#SBATCH -n 7
#SBATCH --mem=2048
#SBATCH --time=00:10:00

srun PA2_Parallel_Static 8192 8192 images/parallel_s.pgm

