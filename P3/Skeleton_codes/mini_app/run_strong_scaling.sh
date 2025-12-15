#!/bin/bash
#SBATCH --job-name=PDE_strong
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --time=02:00:00
#SBATCH --output=strong_scaling_%j.out
#SBATCH --error=strong_scaling_%j.err

module load gcc

make clean
make

declare -a PROBLEM_SIZES=(64 128 256 512 1024)
declare -a THREAD_COUNTS=(1 2 4 8 16)

NT=100
T=0.005

echo "======================================================================"
echo "Strong Scaling Experiment for Mini-Stencil"
echo "======================================================================"
echo ""

# Run experiments for each problem size
for nx in "${PROBLEM_SIZES[@]}"
do
    echo "======================================================================"
    echo "PROBLEM SIZE: nx=$nx, nt=$NT, t=$T"
    echo "======================================================================"
    echo ""
    
    # Arrays to store results for this problem size
    declare -a THREADS
    declare -a TIMES
    
    # Run with different thread counts
    for threads in "${THREAD_COUNTS[@]}"
    do
        export OMP_NUM_THREADS=$threads
        
        echo "----------------------------------------------------------------------"
        echo "Running with $threads threads"
        echo "----------------------------------------------------------------------"
        
        # Run and capture output
        output=$(./main $nx $NT $T)
        
        # Display full output
        echo "$output"
        echo ""
        
        # Extract time from output (look for "simulation took X seconds")
        time=$(echo "$output" | grep "simulation took" | awk '{print $3}')
        
        # Store in arrays
        THREADS+=($threads)
        TIMES+=($time)
        
        echo "Time recorded: $time seconds"
        echo ""
    done
    
    echo "======================================================================"
    echo "SUMMARY FOR nx=$nx"
    echo "======================================================================"
    echo "Threads,Time(s),Speedup,Efficiency"
    
    T1=${TIMES[0]}
    
    for i in "${!THREADS[@]}"; do
        t=${THREADS[$i]}
        time=${TIMES[$i]}
        
        # Compute speedup and efficiency
        speedup=$(awk -v t1=$T1 -v t=$time 'BEGIN{printf "%.4f", t1/t}')
        efficiency=$(awk -v s=$speedup -v t=$t 'BEGIN{printf "%.4f", s/t}')
        
        echo "$t,$time,$speedup,$efficiency"
    done
    echo ""
    echo ""
    
    # Clear arrays for next problem size
    unset THREADS
    unset TIMES
done

