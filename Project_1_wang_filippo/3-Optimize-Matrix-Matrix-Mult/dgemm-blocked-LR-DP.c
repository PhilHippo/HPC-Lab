#include <stdlib.h>

#define BLOCK_SIZE 32

const char* dgemm_desc = "Blocked dgemm with Loop Reordering and Data Packing.";

int min(int a, int b) {
  return a < b ? a : b;
}

static void do_block(int n, int si, int sj, int sk, double* packedA, double* packedB, double* C) {
  int i_end = min(si + BLOCK_SIZE, n);
  int j_end = min(sj + BLOCK_SIZE, n);
  int k_end = min(sk + BLOCK_SIZE, n);

  for (int k = sk; k < k_end; ++k) {
    for (int j = sj; j < j_end; ++j) {
      // B is read from packedB which is contiguous
      double b_val = packedB[(k - sk) + (j - sj) * BLOCK_SIZE];
      for (int i = si; i < i_end; ++i) {
        // same for A
        C[i + j * n] += packedA[(i - si) + (k - sk) * BLOCK_SIZE] * b_val;
      }
    }
  }
}

void square_dgemm(int n, double* A, double* B, double* C) {
  // packed buffers for A and B blocks
  double* packedA = (double*)malloc(BLOCK_SIZE * BLOCK_SIZE * sizeof(double));
  double* packedB = (double*)malloc(BLOCK_SIZE * BLOCK_SIZE * sizeof(double));

  for (int i = 0; i < n; i += BLOCK_SIZE) {
    for (int j = 0; j < n; j += BLOCK_SIZE) {
      for (int k = 0; k < n; k += BLOCK_SIZE) {

        // Pack the current block of A.
        for (int col = 0; col < BLOCK_SIZE; ++col) {
          for (int row = 0; row < BLOCK_SIZE; ++row) {
            if ((i + row < n) && (k + col < n)) {
              packedA[row + col * BLOCK_SIZE] = A[(i + row) + (k + col) * n];
            }
          }
        }

        // Pack the current block of B.
        for (int col = 0; col < BLOCK_SIZE; ++col) {
          for (int row = 0; row < BLOCK_SIZE; ++row) {
            if ((k + row < n) && (j + col < n)) {
              packedB[row + col * BLOCK_SIZE] = B[(k + row) + (j + col) * n];
            }
          }
        }
        
        do_block(n, i, j, k, packedA, packedB, C);

      }
    }
  }
  
  free(packedA);
  free(packedB);
}