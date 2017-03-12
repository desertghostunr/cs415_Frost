#!/bin/bash
#SBATCH --nodes=2
#SBATCH --mem=2048
#SBATCH --time=00:1:00

srun PA1 1 30

