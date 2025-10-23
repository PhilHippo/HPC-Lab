#!/bin/bash
#SBATCH --job-name=mandelbrot
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --time=02:00:00
#SBATCH --output=mandelbrot_%j.out
#SBATCH --error=mandelbrot_%j.err

make clean
make


# Store results in arrays
declare -a THREADS
declare -a TIMES

# Run with different thread counts
for threads in 1 2 4 8 16 20
do

  echo "Running with $threads threads----------------------------"

  
  # Run and capture output
  output=$(./mandel_seq $threads)
  
  # Display full output
  echo "$output"
  echo ""
  
  # Extract time from the gnuplot line
  time=$(echo "$output" | grep "^$threads" | awk '{print $2}')
  
  # Store in arrays
  THREADS+=($threads)
  TIMES+=($time)
done

echo "SUMMARY-------------------------------"
echo "Threads,Time(s),Speedup,Efficiency"

T1=${TIMES[0]}

for i in "${!THREADS[@]}"; do
  t=${THREADS[$i]}
  time=${TIMES[$i]}
  
  # compute speedup and efficiency
  speedup=$(awk -v t1=$T1 -v t=$time 'BEGIN{printf "%.4f", t1/t}')
  efficiency=$(awk -v s=$speedup -v t=$t 'BEGIN{printf "%.4f", s/t}')
  
  echo "$t,$time,$speedup,$efficiency"
done
