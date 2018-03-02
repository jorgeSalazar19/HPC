#!/bin/bash
#
#SBATCH --job-name=omp_matrix_mult
#SBATCH --output=res_matrix_mul.out
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=100

export OMP_MUN_THREADS=$SLURM_CPUS_PER_TASK

./omp_mult

