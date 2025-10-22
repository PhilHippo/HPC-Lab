#define BLOCK_SIZE 32

const char* dgemm_desc = "Blocked dgemm.";

int min(int a, int b) {
  return a < b ? a : b;
}

static void do_block(int n, int si, int sj, int sk, double* A, double* B, double* C) {
  // handles matrices whose dimensions are not a multiple of BLOCK_SIZE
  int i_end = min(si + BLOCK_SIZE, n);
  int j_end = min(sj + BLOCK_SIZE, n);
  int k_end = min(sk + BLOCK_SIZE, n);
  
  for (int i = si; i < i_end; ++i) {
    for (int j = sj; j < j_end; ++j) {
      for (int k = sk; k < k_end; ++k) {
        C[i + j * n] += A[i + k * n] * B[k + j * n];
      }
    }
  }
}

void square_dgemm(int n, double* A, double* B, double* C) {
  for (int i = 0; i < n; i += BLOCK_SIZE) {
    for (int j = 0; j < n; j += BLOCK_SIZE) {
      for(int k = 0; k < n; k += BLOCK_SIZE) {
        do_block(n, i, j, k, A, B, C);
        }
      }
    }
  }