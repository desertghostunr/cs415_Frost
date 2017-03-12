#!/bin/bash
sbatch Sequential.sh 512 512
sbatch Sequential.sh 2048 2048
sbatch Sequential.sh 8192 8192
sbatch Sequential.sh 16384 16384
sbatch Sequential.sh 32768 32768
