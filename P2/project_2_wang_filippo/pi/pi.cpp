#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
  long int N = 10000000000; // <<< carefull! This larger than regular int (its a long int)
  double dx = 1./double(N);
  double sum= 0;
  double pi_serial = 0;
  double pi_parallel = 0;
  double time_serial = 0;
  double time_parallel = 0;
  int num_threads = atoi(argv[1]);

  // Serial version
  time_serial = omp_get_wtime();
  for (long int i = 0; i < N; i++) {
    double x = (i + 0.5) * dx;
    sum += 4.0 / (1.0 + x * x);
  }
  pi_serial = sum * dx;
  time_serial = omp_get_wtime() - time_serial;

  // Reduction
  omp_set_num_threads(num_threads);
  sum = 0.;
  time_parallel = omp_get_wtime();
  #pragma omp parallel for reduction(+:sum)
  for (long int i = 0; i < N; i++) {
    double x = (i + 0.5) * dx;
    sum += 4.0 / (1.0 + x * x);
  }
  pi_parallel = sum * dx;
  time_parallel = omp_get_wtime() - time_parallel;

  double speedup = time_serial / time_parallel;
  printf("pi_srl=%.6e pi_red=%.6e n_threads=%d time_srl=%.6f time_red=%.6f speedup=%.6f\n", pi_serial, pi_parallel, num_threads, time_serial, time_parallel, speedup);

  return 0;
}
