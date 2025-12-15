#!/usr/bin/env python3
"""
Plot runtime vs mesh size (log-log scale) for Poisson solvers.
"""

import matplotlib
matplotlib.use('Agg')  # Non-interactive backend for saving figures
import matplotlib.pyplot as plt
import numpy as np

# Data from runtime_data.csv
# n, N, petsc, sp_dir, dn_dir, sp_cg
n = np.array([8, 16, 32, 64, 128, 256, 512])
N = n * n  # Number of unknowns

petsc = np.array([0.000043, 0.000103, 0.000495, 0.003363, 0.026218, 0.209421, 2.191284])
sp_dir = np.array([0.000369, 0.000871, 0.003368, 0.020089, 0.077452, 0.440396, 3.051764])
sp_cg = np.array([0.000671, 0.001416, 0.002581, 0.008323, 0.058175, 0.404967, 3.464783])

# Dense direct only for smaller sizes (n <= 128)
n_dense = np.array([8, 16, 32, 64, 128])
N_dense = n_dense * n_dense
dn_dir = np.array([0.000153, 0.001224, 0.041698, 1.793762, 12.528147])

# Create figure
plt.figure(figsize=(10, 7))

# Plot each method
plt.loglog(N, petsc, 'o-', label='PETSc (GMRES)', linewidth=2, markersize=8)
plt.loglog(N, sp_dir, 's-', label='Python Sparse Direct', linewidth=2, markersize=8)
plt.loglog(N, sp_cg, '^-', label='Python CG', linewidth=2, markersize=8)
plt.loglog(N_dense, dn_dir, 'd-', label='Python Dense Direct', linewidth=2, markersize=8)

# Add reference lines for scaling
N_ref = np.array([64, 262144])
# O(N) reference
plt.loglog(N_ref, 1e-6 * N_ref, 'k--', alpha=0.3, label=r'$O(N)$')
# O(N^2) reference
plt.loglog(N_ref, 1e-9 * N_ref**2, 'k:', alpha=0.3, label=r'$O(N^2)$')
# O(N^3) reference
plt.loglog(N_ref, 1e-9 * N_ref**3, 'k-.', alpha=0.3, label=r'$O(N^3)$')

# Labels and formatting
plt.xlabel('Number of unknowns $N = n \\times n$', fontsize=12)
plt.ylabel('Solve time (seconds)', fontsize=12)
plt.title('Poisson Solver Performance: Runtime vs Problem Size', fontsize=14)
plt.legend(loc='upper left', fontsize=10)
plt.grid(True, which='both', linestyle='-', alpha=0.3)
plt.tight_layout()

# Save figure
plt.savefig('runtime_plot.pdf')
print("Saved runtime_plot.pdf")
