#!/bin/bash
#SBATCH --nodes=2
#SBATCH --mem=2048
#SBATCH --time=00:15:00

srun PA1 10000 30 10000
