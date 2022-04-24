#!/usr/bin/env bash
#Serial TETTS
sbatch -N 1 --ntasks-per-node=2 -t 40 --gres=gpu:1 ./slurmMain.sh 0

#MPI TESTS
sbatch -N 1 --ntasks-per-node=2 -t 40 --gres=gpu:1 ./slurmMain.sh 1
sbatch -N 1 --ntasks-per-node=4 -t 30 --gres=gpu:1 ./slurmMain.sh 1
sbatch -N 1 --ntasks-per-node=8 -t 20 --gres=gpu:1 ./slurmMain.sh 1
sbatch -N 1 --ntasks-per-node=16 -t 10 --gres=gpu:1 ./slurmMain.sh 1
sbatch -N 1 --ntasks-per-node=32 -t 10 --gres=gpu:1 ./slurmMain.sh 1
sbatch -N 2 --ntasks-per-node=32 -t 10 --gres=gpu:1 ./slurmMain.sh 1
sbatch -N 4 --ntasks-per-node=32 -t 10 --gres=gpu:1 ./slurmMain.sh 1

#CUDA TESTS
sbatch -N 1 --ntasks-per-node=2 -t 10 --gres=gpu:1 ./slurmMain.sh 2
sbatch -N 1 --ntasks-per-node=3 -t 5 --gres=gpu:2 ./slurmMain.sh 2
sbatch -N 1 --ntasks-per-node=7 -t 2 --gres=gpu:6 ./slurmMain.sh 2
sbatch -N 2 --ntasks-per-node=7 -t 2 --gres=gpu:6 ./slurmMain.sh 2
sbatch -N 4 --ntasks-per-node=7 -t 2 --gres=gpu:6 ./slurmMain.sh 2
