#include <omp.h>
#include "walltime.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <fstream>

#define NUM_ITERATIONS 100
#define EPSILON 0.1

using namespace std;

// store benchmark results
struct BenchmarkResult {
  int N;
  int num_threads;
  double time_serial;
  double time_reduction;
  double time_critical;
};

// run dot product for given N and num_threads
BenchmarkResult run_benchmark(int N, int num_threads) {
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
  #pragma omp barrier
  time_red = wall_time() - time_start;

  // Parallel version using critical
  time_start = wall_time();
  for (int iterations = 0; iterations < 5; iterations++) {
    alpha_parallel = 0.0;
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
      #pragma omp critical
      alpha_parallel += a[i] * b[i];
    }
  }
  time_critical = (wall_time() - time_start) * 20; //manually multiplying the time taken to match the number of iterations

  if ((fabs(alpha_parallel - alpha) / fabs(alpha_parallel)) > EPSILON) {
    cout << "parallel reduction: " << alpha_parallel << ", serial: " << alpha
         << "\n";
    cerr << "Alpha not yet implemented correctly!\n";
    exit(1);
  }

  cout << "N=" << N << ", threads=" << num_threads 
       << ", serial=" << time_serial << "s, reduction=" << time_red 
       << "s, critical=" << time_critical << "s" << endl;

  delete[] a;
  delete[] b;
  
  BenchmarkResult result;
  result.N = N;
  result.num_threads = num_threads;
  result.time_serial = time_serial;
  result.time_reduction = time_red;
  result.time_critical = time_critical;
  
  return result;
}

int main() {
  vector<int> N_values = {100000, 1000000, 10000000, 100000000, 1000000000};
  vector<int> thread_counts = {1, 2, 4, 8, 16, 20};

  // for gnuplot
  ofstream outfile("scaling.dat");

  // Write headers
  outfile << "# N threads serial_time reduction_time critical_time speedup_reduction speedup_critical efficiency_reduction efficiency_critical" << endl;

  // run for all combinations
  for (int N : N_values) {
    vector<BenchmarkResult> results;
    
    // Collect all results for this problem size
    for (int threads : thread_counts) {
      results.push_back(run_benchmark(N, threads));
    }
    
    // baseline (1 thread) times for speedup calculation
    double baseline_reduction = results[0].time_reduction;
    double baseline_critical = results[0].time_critical;
    
    // Write results with speedup and efficiency
    for (const auto& res : results) {
      double speedup_reduction = baseline_reduction / res.time_reduction;
      double speedup_critical = baseline_critical / res.time_critical;
      double efficiency_reduction = speedup_reduction / res.num_threads;
      double efficiency_critical = speedup_critical / res.num_threads;
      
      outfile << res.N << " " 
              << res.num_threads << " " 
              << res.time_serial << " " 
              << res.time_reduction << " " 
              << res.time_critical << " "
              << speedup_reduction << " "
              << speedup_critical << " "
              << efficiency_reduction << " "
              << efficiency_critical << endl;
    }
    outfile << endl;
  }

  outfile.close();
  cout << "Done! Results saved to scaling.dat" << endl;

  return 0;
}