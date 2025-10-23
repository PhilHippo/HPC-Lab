#!/usr/bin/gnuplot

set terminal png size 800,600 font "Arial,14"
set output 'mandel_combined_speedup.png'

set xlabel "Number of Threads"
set ylabel "Speedup"
set title "Mandelbrot Strong Scaling"
set xtics (1, 2, 4, 8, 16, 20)
set grid
set key bottom right

plot 'scaling_approx.dat' using 1:3 with linespoints linewidth 3 pointtype 7 pointsize 1.5 lc rgb "blue" title 'Approximate', \
     'scaling_exact.dat' using 1:3 with linespoints linewidth 3 pointtype 9 pointsize 1.5 lc rgb "green" title 'Exact', \
     x with lines linewidth 2 dashtype 2 lc rgb "red" title 'Ideal Speedup'
