#!/bin/bash
#SBATCH -n 1
#SBATCH --mem=2048
#SBATCH --time=00:10:00

./PA2_Sequential 16384 16384 images/sequential.pgm
