#!/usr/bin/gnuplot

# Column layout: N threads serial_time reduction_time critical_time speedup_reduction speedup_critical efficiency_reduction efficiency_critical
# Columns:       1   2        3           4              5              6                 7                8                    9


set terminal png size 1200,800 font "Arial,12"
set logscale y
set logscale x
set xlabel "Number of Threads"
set xtics (1, 2, 4, 8, 16, 20)
set grid

# Plot 1: Reduction Speedup
set output 'reduction_speedup.png'
set ylabel "Speedup"
set title "Strong Scaling: Reduction Speedup (log-log scale)"
set key top left

plot 'scaling.dat' using ($1==100000?$2:1/0):6 with linespoints linewidth 2 pointtype 7 pointsize 1.2 title 'N=10^5', \
     'scaling.dat' using ($1==1000000?$2:1/0):6 with linespoints linewidth 2 pointtype 9 pointsize 1.2 title 'N=10^6', \
     'scaling.dat' using ($1==10000000?$2:1/0):6 with linespoints linewidth 2 pointtype 11 pointsize 1.2 title 'N=10^7', \
     'scaling.dat' using ($1==100000000?$2:1/0):6 with linespoints linewidth 2 pointtype 13 pointsize 1.2 title 'N=10^8', \
     'scaling.dat' using ($1==1000000000?$2:1/0):6 with linespoints linewidth 2 pointtype 15 pointsize 1.2 title 'N=10^9', \
     'scaling.dat' using 2:2 with lines linewidth 2 dashtype 2 linecolor rgb "black" title 'Ideal Speedup'

print "Reduction speedup plot saved to reduction_speedup.png"

# Plot 2: Critical Speedup
set output 'critical_speedup.png'
set ylabel "Speedup"
set title "Strong Scaling: Critical Speedup (log-log scale)"
set key top left

plot 'scaling.dat' using ($1==100000?$2:1/0):7 with linespoints linewidth 2 pointtype 7 pointsize 1.2 title 'N=10^5', \
     'scaling.dat' using ($1==1000000?$2:1/0):7 with linespoints linewidth 2 pointtype 9 pointsize 1.2 title 'N=10^6', \
     'scaling.dat' using ($1==10000000?$2:1/0):7 with linespoints linewidth 2 pointtype 11 pointsize 1.2 title 'N=10^7', \
     'scaling.dat' using ($1==100000000?$2:1/0):7 with linespoints linewidth 2 pointtype 13 pointsize 1.2 title 'N=10^8', \
     'scaling.dat' using ($1==1000000000?$2:1/0):7 with linespoints linewidth 2 pointtype 15 pointsize 1.2 title 'N=10^9', \
     'scaling.dat' using 2:2 with lines linewidth 2 dashtype 2 linecolor rgb "black" title 'Ideal Speedup'

print "Critical speedup plot saved to critical_speedup.png"

# Plot 3: Reduction Efficiency
set output 'reduction_efficiency.png'
unset logscale y
set ylabel "Parallel Efficiency"
set title "Strong Scaling: Reduction Efficiency"
set yrange [0:1.2]
set key top right

plot 'scaling.dat' using ($1==100000?$2:1/0):8 with linespoints linewidth 2 pointtype 7 pointsize 1.2 title 'N=10^5', \
     'scaling.dat' using ($1==1000000?$2:1/0):8 with linespoints linewidth 2 pointtype 9 pointsize 1.2 title 'N=10^6', \
     'scaling.dat' using ($1==10000000?$2:1/0):8 with linespoints linewidth 2 pointtype 11 pointsize 1.2 title 'N=10^7', \
     'scaling.dat' using ($1==100000000?$2:1/0):8 with linespoints linewidth 2 pointtype 13 pointsize 1.2 title 'N=10^8', \
     'scaling.dat' using ($1==1000000000?$2:1/0):8 with linespoints linewidth 2 pointtype 15 pointsize 1.2 title 'N=10^9', \
     1 with lines linewidth 2 dashtype 2 linecolor rgb "black" title 'Ideal Efficiency (100%)'

print "Reduction efficiency plot saved to reduction_efficiency.png"

# Plot 4: Critical Efficiency
set output 'critical_efficiency.png'
set ylabel "Parallel Efficiency"
set title "Strong Scaling: Critical Efficiency"
set yrange [0:1.2]
set key top right

plot 'scaling.dat' using ($1==100000?$2:1/0):9 with linespoints linewidth 2 pointtype 7 pointsize 1.2 title 'N=10^5', \
     'scaling.dat' using ($1==1000000?$2:1/0):9 with linespoints linewidth 2 pointtype 9 pointsize 1.2 title 'N=10^6', \
     'scaling.dat' using ($1==10000000?$2:1/0):9 with linespoints linewidth 2 pointtype 11 pointsize 1.2 title 'N=10^7', \
     'scaling.dat' using ($1==100000000?$2:1/0):9 with linespoints linewidth 2 pointtype 13 pointsize 1.2 title 'N=10^8', \
     'scaling.dat' using ($1==1000000000?$2:1/0):9 with linespoints linewidth 2 pointtype 15 pointsize 1.2 title 'N=10^9', \
     1 with lines linewidth 2 dashtype 2 linecolor rgb "black" title 'Ideal Efficiency (100%)'

print "Critical efficiency plot saved to critical_efficiency.png"
