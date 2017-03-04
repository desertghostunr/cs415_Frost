#!/bin/bash
#SBATCH -n 1
#SBATCH --mem=2048
#SBATCH --time=00:10:00

./PA2_Sequential 512 512 sequential.pgm
