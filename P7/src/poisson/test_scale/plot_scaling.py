#!/usr/bin/env python3
"""
Plot strong scaling results: runtime vs number of processes (log-log scale).
"""

import matplotlib
matplotlib.use('Agg')  # Non-interactive backend for saving figures
import matplotlib.pyplot as plt
import numpy as np

# Strong scaling data for 1024x1024 grid
# Number of MPI processes
processes = np.array([1, 2, 4, 8, 16])

# Solve time (seconds) from test results
solve_time = np.array([18.655954, 10.780487, 5.321828, 3.030353, 1.426906])

# Total time (RHS + Assembly + Solve)
assembly_time = np.array([0.599025, 0.331290, 0.162303, 0.101942, 0.077994])
total_time = solve_time + assembly_time

# Ideal scaling (linear speedup from 1 process)
ideal_time = total_time[0] / processes

# Create figure
plt.figure(figsize=(8, 6))

# Plot actual runtime
plt.loglog(processes, total_time, 'o-', label='PETSc CG (measured)', linewidth=2, markersize=10, color='blue')

# Plot ideal scaling
plt.loglog(processes, ideal_time, 'k--', label='Ideal scaling', linewidth=2, alpha=0.7)

# Labels and formatting
plt.xlabel('Number of MPI processes', fontsize=12)
plt.ylabel('Total runtime (seconds)', fontsize=12)
plt.title('Strong Scaling: PETSc Poisson Solver ($1024 \\times 1024$ grid)', fontsize=14)
plt.legend(loc='upper right', fontsize=11)
plt.grid(True, which='both', linestyle='-', alpha=0.3)
plt.xticks(processes, processes)
plt.tight_layout()

# Save figure
plt.savefig('scaling_plot.pdf')
print("Saved scaling_plot.pdf")

# Print speedup and efficiency
print("\nStrong Scaling Results (1024x1024 grid):")
print("-" * 50)
print(f"{'Procs':<8} {'Time (s)':<12} {'Speedup':<10} {'Efficiency':<10}")
print("-" * 50)
for i, p in enumerate(processes):
    speedup = total_time[0] / total_time[i]
    efficiency = speedup / p * 100
    print(f"{p:<8} {total_time[i]:<12.3f} {speedup:<10.2f} {efficiency:<10.1f}%")
