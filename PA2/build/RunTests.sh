#!/bin/bash
sbatch Sequential.sh 512 512
sbatch -n 3 Static.sh 512 512
sbatch -n 3 Dynamic.sh 512 512
sbatch -n 9 Static.sh 9 512 512
sbatch -n 9 Dynamic.sh 9 512 512
sbatch -n 13 Static.sh 512 512
sbatch -n 13 Dynamic.sh 512 512
sbatch Static.sh 16 512 512
sbatch Dynamic.sh 16 512 512
sbatch Static.sh 17 512 512
sbatch Dynamic.sh 17 512 512
sbatch Static.sh 32 512 512
sbatch Dynamic.sh 32 512 512
sbatch Static.sh 33 512 512
sbatch Dynamic.sh 33 512 512
sbatch Sequential.sh 2048 2048
sbatch -n 3 Static.sh 2048 2048
sbatch -n 3 Dynamic.sh 2048 2048
sbatch -n 9 Static.sh 2048 2048
sbatch -n 9 Dynamic.sh 2048 2048
sbatch -n 13 Static.sh 2048 2048
sbatch -n 13 Dynamic.sh 2048 2048
sbatch Static.sh 16 2048 2048
sbatch Dynamic.sh 16 2048 2048
sbatch Static.sh 17 2048 2048
sbatch Dynamic.sh 17 2048 2048
sbatch Static.sh 32 2048 2048
sbatch Dynamic.sh 32 2048 2048
sbatch Static.sh 33 2048 2048
sbatch Dynamic.sh 33 2048 2048
sbatch Sequential.sh 8192 8192
sbatch -n 3 Static.sh 8192 8192
sbatch -n 3 Dynamic.sh 8192 8192
sbatch -n 9 Static.sh 8192 8192
sbatch -n 9 Dynamic.sh 8192 8192
sbatch -n 13 Static.sh 8192 8192
sbatch -n 13 Dynamic.sh 8192 8192
sbatch Static.sh 16 8192 8192
sbatch Dynamic.sh 16 8192 8192
sbatch Static.sh 17 8192 8192
sbatch Dynamic.sh 17 8192 8192
sbatch Static.sh 32 8192 8192
sbatch Dynamic.sh 32 8192 8192
sbatch Static.sh 33 8192 8192
sbatch Dynamic.sh 33 8192 8192
sbatch Sequential.sh 16384 16384
sbatch -n 3 Static.sh 16384 16384
sbatch -n 3 Dynamic.sh 16384 16384
sbatch -n 9 Static.sh 16384 16384
sbatch -n 9 Dynamic.sh 16384 16384
sbatch Static.sh 13 16384 16384
sbatch Dynamic.sh 13 16384 16384
sbatch Static.sh 16 16384 16384
sbatch Dynamic.sh 16 16384 16384
sbatch Static.sh 17 16384 16384
sbatch Dynamic.sh 17 16384 16384
sbatch Static.sh 32 16384 16384
sbatch Dynamic.sh 32 16384 16384
sbatch Static.sh 33 16384 16384
sbatch Dynamic.sh 33 16384 16384
sbatch Sequential.sh 32768 32768
sbatch -n 3 Static.sh 32768 32768
sbatch -n 3 Dynamic.sh 32768 32768
sbatch -n 9 Static.sh 32768 32768
sbatch -n 9 Dynamic.sh 32768 32768
sbatch Static.sh 13 32768 32768
sbatch Dynamic.sh 13 32768 32768
sbatch Static.sh 16 32768 32768
sbatch Dynamic.sh 16 32768 32768
sbatch Static.sh 17 32768 32768
sbatch Dynamic.sh 17 32768 32768
sbatch Static.sh 32 32768 32768
sbatch Dynamic.sh 32 32768 32768
sbatch Static.sh 33 32768 32768
sbatch Dynamic.sh 33 32768 32768
