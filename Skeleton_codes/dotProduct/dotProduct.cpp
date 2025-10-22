#include <omp.h>
#include "walltime.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <fstream>

#define NUM_ITERATIONS 100

// Example benchmarks
// 0.008s ~0.8MB
// #define N 100000
// 0.1s ~8MB
// #define N 1000000
// 1.1s ~80MB
// #define N 10000000
// 13s ~800MB
// #define N 100000000
// 127s 16GB
//#define N 1000000000
#define EPSILON 0.1

using namespace std;

// run dot product for given N and num_threads
void run_benchmark(int N, int num_threads, ofstream& outfile) {
  double time_serial, time_start = 0.0;
  double *a, *b;

  // Allocate memory for the vectors as 1-D arrays
  a = new double[N];
  b = new double[N];

  // Initialize the vectors with some values
  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = i / 10.0;
  }

  long double alpha = 0;
  long double alpha_parallel = 0;
  double time_red = 0;
  double time_critical = 0;

  // Set number of threads
  omp_set_num_threads(num_threads);

  // Serial execution
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha = 0.0;
    for (int i = 0; i < N; i++) {
      alpha += a[i] * b[i];
    }
  }
  time_serial = wall_time() - time_start;

  // Parallel version using reduction
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha_parallel = 0.0;
    #pragma omp parallel for reduction(+:alpha_parallel)
    for (int i = 0; i < N; i++) {
      alpha_parallel += a[i] * b[i];
    }
  }
  time_red = wall_time() - time_start;

  // Parallel version using critical
  int critical_iterations = (N > 1000000) ? 5 : 10;
  time_start = wall_time();
  for (int iterations = 0; iterations < critical_iterations; iterations++) {
    alpha_parallel = 0.0;
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
      #pragma omp critical
      alpha_parallel += a[i] * b[i];
    }
  }
  time_critical = wall_time() - time_start;

  if ((fabs(alpha_parallel - alpha) / fabs(alpha_parallel)) > EPSILON) {
    cout << "parallel reduction: " << alpha_parallel << ", serial: " << alpha
         << "\n";
    cerr << "Alpha not yet implemented correctly!\n";
    exit(1);
  }

  // Output for gnuplot
  outfile << N << " " << num_threads << " " << time_serial << " " 
          << time_red << " " << time_critical << endl;

  cout << "N=" << N << ", threads=" << num_threads 
       << ", serial=" << time_serial << "s, reduction=" << time_red 
       << "s, critical=" << time_critical << "s" << endl;

  delete[] a;
  delete[] b;
}

int main() {
  vector<int> N_values = {100000, 1000000, 10000000, 100000000, 1000000000};

  vector<int> thread_counts = {1, 2, 4, 8, 16, 20};

  // for gnuplot
  ofstream outfile("scaling.dat");

  // Write header for gnuplot
  outfile << "# N threads serial_time reduction_time critical_time speedup_reduction speedup_critical" << endl;

  cout << "Starting strong scaling analysis..." << endl;
  cout << "Vector lengths: ";
  for (int n : N_values) cout << n << " ";
  cout << endl;
  cout << "Thread counts: ";
  for (int t : thread_counts) cout << t << " ";
  cout << endl; 

  // run for all combinations
  for (int N : N_values) {
    for (int threads : thread_counts) {
      run_benchmark(N, threads, outfile);
    }
    outfile << endl;
  }

  outfile.close();
  cout << "Done! Results saved to scaling.dat" << endl;

  return 0;
}