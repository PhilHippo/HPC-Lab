#!/bin/bash
#SBATCH --job-name=recur      # Job name
#SBATCH --output=recur-%j.out # Output file
#SBATCH --error=recur-%j.err  # Error file
#SBATCH --ntasks=1            # Number of tasks
#SBATCH --cpus-per-task=20    # Number of CPUs per task
#SBATCH --mem-per-cpu=1024    # Memory per CPU
#SBATCH --time=00:10:00       # Wall clock time limit

module load gcc
module list

make clean
make

echo "=== Sequential Version ==========================="
./recur_seq

echo ""
echo "=== Parallel Version =========="
for threads in 1 2 4 8 16 32 64 128
do
  echo ""
  echo "--- Running with $threads threads ---"
  export OMP_NUM_THREADS=$threads
  ./recur_omp
done

