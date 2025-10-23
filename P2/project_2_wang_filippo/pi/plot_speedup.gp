set terminal pngcairo size 800,600 enhanced font 'Verdana,12'
set output 'speedup.png'

set title "Speedup of Parallel π Approximation (N=10^{10})"
set xlabel "Number of Threads"
set ylabel "Speedup"

set logscale x 2
set logscale y 2

set grid

set key top left

# Ideal speedup line
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5
set style line 2 lc rgb '#dd181f' lt 2 lw 2

plot 'pi.dat' using 1:4 with linespoints ls 1 title 'Speedup', \
     x with lines ls 2 title 'Ideal Speedup'