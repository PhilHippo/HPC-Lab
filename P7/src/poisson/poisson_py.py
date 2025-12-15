"""
 * HPC Assignment: Python Poisson Equation Solver
 * Author: Aryan Eftekhari
 *
 * Description:
 * This Python program solves the 2D Poisson equation using finite difference methods. 
 * It constructs a sparse Laplacian matrix and solves the resulting linear system 
 * using various solvers (e.g., sparse direct, dense direct, and conjugate gradient methods).
 * 
 * The equation solved is:
 *     -Δu = f, 0 < x, y < 1
 * with:
 *     - Dirichlet boundary conditions (u = 0 on all boundaries).
 *     - Forcing function f = constant.
 *
 * Fix Suggestion:
 * Ensure input arguments are validated for edge cases. Include more solver options and refine grid scaling for large `nx` and `ny`.
 *
 * Compilation Instructions:
 * No compilation required. Ensure required Python libraries (`numpy`, `scipy`, `argparse`) are installed.
 * 
 * Run Instructions:
 * Execute the script with desired parameters. Example:
 *     python poisson_solver.py -nx 64 -ny 64 -solver sp_dir,dn_dir,sp_cg
 * where sp_dir:  Sparse Direct Solver, dn_dir: Dense Direct Solver, sp_cg: Conjugate Gradient Solver
 """

import numpy as np
from scipy.sparse import csr_matrix
from scipy.sparse.linalg import spsolve, cg
from numpy.linalg import solve
import time
import argparse

def ComputeRHS(nx, ny, f_value):
    """Compute the right-hand side vector."""
    rhs = np.full(nx * ny, f_value)
    return rhs

def ComputeMatrix(nx, ny, dx, dy):
    """
    Construct the sparse Laplacian matrix using the 5-point stencil.
    """
    N = nx * ny
    
    # Coefficients for the 5-point stencil
    cx = 1.0 / (dx * dx)  # coefficient for x-direction neighbors
    cy = 1.0 / (dy * dy)  # coefficient for y-direction neighbors
    diag_val = 2.0 * cx + 2.0 * cy  # diagonal coefficient
    
    # Pre-compute number of non-zeros
    n_diag = N
    n_x_neighbors = 2 * (nx - 1) * ny  # left-right pairs, no wrap at row boundaries
    n_y_neighbors = 2 * nx * (ny - 1)  # bottom-top pairs
    nnz = n_diag + n_x_neighbors + n_y_neighbors
    
    # Pre-allocate arrays
    data = np.empty(nnz, dtype=np.float64)
    row_indices = np.empty(nnz, dtype=np.int32)
    col_indices = np.empty(nnz, dtype=np.int32)
    
    idx = 0
    
    # 1. Diagonal entries (all N points)
    all_k = np.arange(N, dtype=np.int32)
    row_indices[idx:idx+n_diag] = all_k
    col_indices[idx:idx+n_diag] = all_k
    data[idx:idx+n_diag] = diag_val
    idx += n_diag
    
    # 2. x-direction neighbors: k and k+1 are neighbors IFF (k+1) % nx != 0
    # i.e., they are in the same row
    all_k_except_last_col = all_k[all_k % nx != nx - 1]  # exclude rightmost column
    n_x_pairs = len(all_k_except_last_col)
    
    # Entry (k, k+1): right neighbor (upper diagonal entries)
    row_indices[idx:idx+n_x_pairs] = all_k_except_last_col
    col_indices[idx:idx+n_x_pairs] = all_k_except_last_col + 1
    data[idx:idx+n_x_pairs] = -cx
    idx += n_x_pairs
    
    # Entry (k+1, k): left neighbor (symmetric)  (lower diagonal entries)
    row_indices[idx:idx+n_x_pairs] = all_k_except_last_col + 1
    col_indices[idx:idx+n_x_pairs] = all_k_except_last_col
    data[idx:idx+n_x_pairs] = -cx
    idx += n_x_pairs
    
    # 3. y-direction neighbors: k and k+nx are neighbors (no boundary issues within grid)
    all_k_except_last_row = all_k[all_k < N - nx]  # exclude top row
    n_y_pairs = len(all_k_except_last_row)
    
    # Entry (k, k+nx): top neighbor (upper off-diagonal entries)
    row_indices[idx:idx+n_y_pairs] = all_k_except_last_row
    col_indices[idx:idx+n_y_pairs] = all_k_except_last_row + nx
    data[idx:idx+n_y_pairs] = -cy
    idx += n_y_pairs
    
    # Entry (k+nx, k): bottom neighbor (symmetric) (lower off-diagonal entries)
    row_indices[idx:idx+n_y_pairs] = all_k_except_last_row + nx
    col_indices[idx:idx+n_y_pairs] = all_k_except_last_row
    data[idx:idx+n_y_pairs] = -cy

    return csr_matrix((data, (row_indices, col_indices)), shape=(N, N))

if __name__ == "__main__":
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="Solve the Poisson equation using different solvers.")
    parser.add_argument("-nx", type=int, default=64, help="Number of grid points in x (and y).")
    parser.add_argument("-ny", type=int, default=18,  help="Number of grid points in y (default: same as nx).")
    parser.add_argument("-solver", type=str, default="sp_dir,dn_dir,sp_cg",  help="Comma-separated list of solvers to use.")
    args = parser.parse_args()

    # Parameters
    nx = args.nx
    ny = args.ny 

    solver_list = args.solver.split(",")
    print("Selected solvers:", solver_list)

    Lx, Ly = 1.0, 1.0  # Domain size
    dx = Lx / (nx - 1) # Grid spacing in x
    dy = Ly / (ny - 1) # Grid spacing in y
    f_value = 20.0     # Right-hand side (constant)

    print("-"*100)
    print(f"Poisson's Equation Solver Python for {nx}x{ny}")

    # Compute RHS and Matrix
    start_rhs = time.time()
    rhs = ComputeRHS(nx, ny, f_value)
    end_rhs = time.time()

    start_matrix = time.time()
    laplacian = ComputeMatrix(nx, ny, dx, dy)
    end_matrix = time.time()

    np.set_printoptions(linewidth=200)

    print("-"*100)
    print(f"{'RHS Time:':<40} {end_rhs - start_rhs:.6f} seconds")
    print(f"{'Matrix Assembly Time:':<40} {end_matrix - start_matrix:.6f} seconds")
    print("-"*100)
    # Solve the system with different methods
    solutions = {}

    if 'sp_dir' in solver_list:
        # Sparse direct solver
        start_sparse_direct = time.time()
        solutions['sp_dir'] = spsolve(laplacian, rhs)
        end_sparse_direct = time.time()
        print(f"{'Sparse Direct Solver Time:':<40} {end_sparse_direct - start_sparse_direct:.6f} seconds, "
            f"Norm of Solution: {np.linalg.norm(solutions['sp_dir'], ord=2):.6f}")

    if 'dn_dir' in solver_list:
        # Dense direct solver
        dense_laplacian = laplacian.toarray()
        start_dense_direct = time.time()
        solutions['dn_dir'] = solve(dense_laplacian, rhs)
        end_dense_direct = time.time()
        print(f"{'Dense Direct Solver Time:':<40} {end_dense_direct - start_dense_direct:.6f} seconds, "
            f"Norm of Solution: {np.linalg.norm(solutions['dn_dir'], ord=2):.6f}")

    if 'sp_cg' in solver_list:
        # Sparse iterative solver (Conjugate Gradient)
        start_cg = time.time()
        solutions['sp_cg'], _ = cg(laplacian, rhs, rtol=1e-6)
        end_cg = time.time()
        print(f"{'Conjugate Gradient (sparse) Solver Time:':<40} {end_cg - start_cg:.6f} seconds, "
            f"Norm of Solution: {np.linalg.norm(solutions['sp_cg'], ord=2):.6f}")

    #Write the solution vector to a text file.
    filename = "solution_py_sp_dir"

    print("-"*100)
    for solver in solver_list:
        filename = "solution_"+solver
        with open(filename+'.dat', 'w') as file:
            file.write("Py Solution\n")
            file.write("============\n")
            np.savetxt(file, solutions[solver], fmt='%.8f')

        with open(filename+'.info', 'w') as file:
            file.write("# nx, ny, a\n")
            file.write(f"{nx},{ny},-\n")

        print(f"Solution written to disk for {filename} ...")
