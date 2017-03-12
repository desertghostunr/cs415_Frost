#!/bin/bash
sbatch -n 17 Dynamic.sh 16384 16384
sleep 5
sbatch -n 17 Static.sh 512 512
sleep 5
sbatch -n 17 Static.sh 512 512
sleep 5
sbatch -n 9 Static.sh 8192 8192
sleep 5
sbatch -n 9 Static.sh 2048 2048
