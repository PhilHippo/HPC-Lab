#!/bin/bash
#SBATCH --job-name=PDE_weak
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --time=02:00:00
#SBATCH --output=weak_scaling_%j.out
#SBATCH --error=weak_scaling_%j.err

module load gcc

make clean
make

declare -a THREAD_COUNTS=(1 2 4 8 16)

BASE_RESOLUTION=32

NT=100
T=0.005

echo "======================================================================"
echo "Weak Scaling Experiment for Mini-Stencil"
echo "Base resolution per thread: ${BASE_RESOLUTION}"
echo "======================================================================"
echo ""

declare -a THREADS
declare -a NX_VALUES
declare -a TIMES

for threads in "${THREAD_COUNTS[@]}"
do
    nx=$(awk -v base=$BASE_RESOLUTION -v t=$threads 'BEGIN{printf "%.0f", base * sqrt(t)}')
    
    export OMP_NUM_THREADS=$threads
    
    echo "----------------------------------------------------------------------"
    echo "Running with $threads threads, nx=$nx"
    echo "----------------------------------------------------------------------"
    
    output=$(./main $nx $NT $T)
    
    echo "$output"
    echo ""
    
    time=$(echo "$output" | grep "simulation took" | awk '{print $3}')
    
    THREADS+=($threads)
    NX_VALUES+=($nx)
    TIMES+=($time)
    
    echo "Time recorded: $time seconds"
    echo ""
done

echo ""
echo "SUMMARY FOR weak scaling"

T1=${TIMES[0]}

for i in "${!THREADS[@]}"; do
    t=${THREADS[$i]}
    nx=${NX_VALUES[$i]}
    time=${TIMES[$i]}
    
    efficiency=$(awk -v t1=$T1 -v t=$time 'BEGIN{printf "%.4f", t1/t}')
    
    echo "$t,$nx,$time,$efficiency"
done

echo ""
echo "======================================================================"
echo "Weak scaling experiment completed"
echo "======================================================================"

