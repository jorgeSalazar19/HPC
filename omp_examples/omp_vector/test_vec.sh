#!/bin/bash
#
#SBATCH --job-name=omp_vector_sum
#SBATCH --output=res_vector_sum.out
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=100

export OMP_MUN_THREADS=$SLURM_CPUS_PER_TASK

./omp_vector

