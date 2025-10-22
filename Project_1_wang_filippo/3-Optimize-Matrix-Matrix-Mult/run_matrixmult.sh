#!/bin/bash
#SBATCH --job-name=matrixmult
#SBATCH --output=matrixmult-%j.out
#SBATCH --error=matrixmult-%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --partition=slim
#SBATCH --time=00:20:00

# load some modules & list loaded modules
module load gcc intel-oneapi-mkl

export OMP_NUM_THREADS=1

echo "==== benchmark-naive ======================"
srun ./benchmark-naive | tee timing_basic_dgemm.data
echo
echo "==== benchmark-blas ======================="
srun ./benchmark-blas | tee timing_blas_dgemm.data
echo
echo "==== benchmark-blocked-naive ===================="
srun ./benchmark-blocked-naive | tee timing_blocked_dgemm-naive.data
echo
echo "==== benchmark-blocked Loop Reordering ===================="
srun ./benchmark-blocked-LR | tee timing_blocked_dgemm_LR.data
echo
echo "==== benchmark-blocked Loop Reordering + Data Packing ===================="
srun ./benchmark-blocked-LR-DP | tee timing_blocked_dgemm_LR_DP.data
echo
echo "==== benchmark-blocked Loop Reordering + Loop Unrolling ===================="
srun ./benchmark-blocked-LR-LU | tee timing_blocked_dgemm_LR_LU.data

echo
echo "==== plot results ========================="
gnuplot timing.gp
