#!/bin/bash
#SBATCH -n 8
#SBATCH --mem=2048
#SBATCH --time=00:10:00

srun PA2_Parallel_Static 16384 16384  parallel_s.pgm

