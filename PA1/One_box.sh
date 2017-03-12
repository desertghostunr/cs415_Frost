#!/bin/bash
#SBATCH -n 2
#SBATCH --mem=2048
#SBATCH --time=00:00:59

srun PA1 1 30
