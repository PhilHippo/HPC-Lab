#!/bin/bash
#SBATCH --job-name=dotProduct
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --time=04:00:00
#SBATCH --output=dotProduct_%j.out
#SBATCH --error=dotProduct_%j.err

make clean
make
./dotProduct


