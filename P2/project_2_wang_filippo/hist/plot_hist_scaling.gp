#!/usr/bin/gnuplot

set terminal png size 1200,500 font "Arial,12"
set output 'hist_scaling_plots.png'

set multiplot layout 1,2

# Plot 1: Strong Scaling (Speedup)
set logscale xy 2
set xlabel "Number of Threads"
set ylabel "Speedup"
set title "Histogram Strong Scaling"
set grid
set key bottom right

set xtics ("2^0" 1, "2^1" 2, "2^2" 4, "2^3" 8, "2^4" 16, "2^5" 32, "2^6" 64, "2^7" 128)

plot 'hist_strong_scaling.dat' using 1:3 with linespoints linewidth 3 pointtype 7 pointsize 1.5 lc rgb "blue" title 'Actual Speedup', \
     x with lines linewidth 2 dashtype 2 lc rgb "red" title 'Ideal Speedup'

# Plot 2: Parallel Efficiency
set ylabel "Parallel Efficiency"
set title "Histogram Parallel Efficiency"
set yrange [0.01:2]
set key top right

plot 'hist_strong_scaling.dat' using 1:4 with linespoints linewidth 3 pointtype 7 pointsize 1.5 lc rgb "green" title 'Efficiency', \
     1 with lines linewidth 2 dashtype 2 lc rgb "red" title 'Ideal Efficiency (100%)'

unset multiplot

