#!/bin/bash
#SBATCH --job-name=pi
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=01:00:00
#SBATCH --output=pi_%j.out
#SBATCH --error=pi_%j.err

make clean
make

for t in 1 2 4 8
do
  ./pi $t 
done

