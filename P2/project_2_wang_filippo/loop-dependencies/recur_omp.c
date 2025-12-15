#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "walltime.h"

int main(int argc, char *argv[]) {
  int N = 2000000000;
  double up = 1.00000001;
  double Sn = 1.00000001;
  int n;

  /* allocate memory for the recursion */
  double *opt = (double *)malloc((N + 1) * sizeof(double));
  if (opt == NULL) {
    perror("failed to allocate problem size");
    exit(EXIT_FAILURE);
  }

  double time_start = walltime();
  double Sn_initial = Sn;
  #pragma omp parallel
  {
    int n;
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();

    // chunk the iterations
    long items_per_thread = (N + 1) / num_threads;
    long start_index = (long)thread_id * items_per_thread;
    long end_index = (thread_id == num_threads - 1) ? (N + 1) : start_index + items_per_thread; // last thread gets the remaining iterations

    // Each thread calculates its starting value for Sn using pow()
    double thread_Sn = Sn_initial * pow(up, start_index);

    for (n = start_index; n < end_index; ++n) {
      opt[n] = thread_Sn;
      thread_Sn *= up;
    }
  }

  // After the loop, calculate the final value of Sn
  Sn = Sn_initial * pow(up, N + 1);

  printf("Parallel RunTime  :  %f seconds\n", walltime() - time_start);
  printf("Final Result Sn   :  %.17g \n", Sn);

  double temp = 0.0;
  for (n = 0; n <= N; ++n) {
    temp += opt[n] * opt[n];
  }
  printf("Result ||opt||^2_2 :  %f\n", temp / (double)N);
  printf("\n");

  return 0;
}
