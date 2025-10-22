#define BLOCK_SIZE 32
#define UNROLL_FACTOR 4

const char* dgemm_desc = "Blocked dgemm with loop unrolling";

int min(int a, int b) {
  return a < b ? a : b;
}

//Assume 32x32 region.
static void do_block_unrolled(int n, int si, int sj, int sk, double* A, double* B, double* C) {
  for (int j = 0; j < BLOCK_SIZE; j += UNROLL_FACTOR) {   
    for (int i = 0; i < BLOCK_SIZE; i += UNROLL_FACTOR) {
      // maps the 4x4 sub-block to registers
      double c_00=0, c_01=0, c_02=0, c_03=0;
      double c_10=0, c_11=0, c_12=0, c_13=0;
      double c_20=0, c_21=0, c_22=0, c_23=0;
      double c_30=0, c_31=0, c_32=0, c_33=0;

      for (int k = 0; k < BLOCK_SIZE; ++k) {
        // loads 4 elements from a column of A
        double a_0 = A[(si + i + 0) + (sk + k) * n];
        double a_1 = A[(si + i + 1) + (sk + k) * n];
        double a_2 = A[(si + i + 2) + (sk + k) * n];
        double a_3 = A[(si + i + 3) + (sk + k) * n];

        // loads 4 elements from a row of B
        double b_0 = B[(sk + k) + (sj + j + 0) * n];
        double b_1 = B[(sk + k) + (sj + j + 1) * n];
        double b_2 = B[(sk + k) + (sj + j + 2) * n];
        double b_3 = B[(sk + k) + (sj + j + 3) * n];
        
        // performs the outer product and accumulates
        c_00 += a_0 * b_0; c_01 += a_0 * b_1; c_02 += a_0 * b_2; c_03 += a_0 * b_3;
        c_10 += a_1 * b_0; c_11 += a_1 * b_1; c_12 += a_1 * b_2; c_13 += a_1 * b_3;
        c_20 += a_2 * b_0; c_21 += a_2 * b_1; c_22 += a_2 * b_2; c_23 += a_2 * b_3;
        c_30 += a_3 * b_0; c_31 += a_3 * b_1; c_32 += a_3 * b_2; c_33 += a_3 * b_3;
      }
      
      // accumulates the results in C
      C[(si+i+0) + (sj+j+0)*n] += c_00; C[(si+i+0) + (sj+j+1)*n] += c_01; C[(si+i+0) + (sj+j+2)*n] += c_02; C[(si+i+0) + (sj+j+3)*n] += c_03;
      C[(si+i+1) + (sj+j+0)*n] += c_10; C[(si+i+1) + (sj+j+1)*n] += c_11; C[(si+i+1) + (sj+j+2)*n] += c_12; C[(si+i+1) + (sj+j+3)*n] += c_13;
      C[(si+i+2) + (sj+j+0)*n] += c_20; C[(si+i+2) + (sj+j+1)*n] += c_21; C[(si+i+2) + (sj+j+2)*n] += c_22; C[(si+i+2) + (sj+j+3)*n] += c_23;
      C[(si+i+3) + (sj+j+0)*n] += c_30; C[(si+i+3) + (sj+j+1)*n] += c_31; C[(si+i+3) + (sj+j+2)*n] += c_32; C[(si+i+3) + (sj+j+3)*n] += c_33;
    }
  }
}


static void do_block_edge(int n, int si, int sj, int sk, double* A, double* B, double* C) {
  int i_end = min(si + BLOCK_SIZE, n);
  int j_end = min(sj + BLOCK_SIZE, n);
  int k_end = min(sk + BLOCK_SIZE, n);
  
  for (int k = sk; k < k_end; ++k) {
    for (int j = sj; j < j_end; ++j) {
      double b_val = B[k + j * n];
      for (int i = si; i < i_end; ++i) {
        C[i + j * n] += A[i + k * n] * b_val;
      }
    }
  }
}

void square_dgemm(int n, double* A, double* B, double* C) {
  for (int i = 0; i < n; i += BLOCK_SIZE) {
    for (int j = 0; j < n; j += BLOCK_SIZE) {
      for (int k = 0; k < n; k += BLOCK_SIZE) {  
        // handles partial blocks at edges
        if (i + BLOCK_SIZE <= n && j + BLOCK_SIZE <= n && k + BLOCK_SIZE <= n) {
          do_block_unrolled(n, i, j, k, A, B, C);
        } else {
          do_block_edge(n, i, j, k, A, B, C);
        }
      }
    }
  }
}