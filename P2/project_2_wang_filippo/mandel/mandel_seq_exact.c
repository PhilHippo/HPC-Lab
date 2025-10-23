#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "consts.h"
#include "pngwriter.h"
#include "walltime.h"

int main(int argc, char **argv) {
  png_data *pPng = png_create(IMAGE_WIDTH, IMAGE_HEIGHT);

  double x, y, x2, y2, cx, cy;

  double fDeltaX = (MAX_X - MIN_X) / (double)IMAGE_WIDTH;
  double fDeltaY = (MAX_Y - MIN_Y) / (double)IMAGE_HEIGHT;

  long nTotalIterationsCount = 0;

  int num_threads = atoi(argv[1]);
  omp_set_num_threads(num_threads);

  long i, j;

  double time_start = walltime();
  // do the calculation
  #pragma omp parallel for private(i, cx, cy, x, y, x2, y2) reduction(+:nTotalIterationsCount) schedule(dynamic)
  for (j = 0; j < IMAGE_HEIGHT; j++) {
    // more precise calculation of cy (instead of cy = MIN_Y + j * fDeltaY)
    cy = MIN_Y;
    for (int k=0; k<j; k++) {
       cy += fDeltaY;
       } 
    cx = MIN_X;
    for (i = 0; i < IMAGE_WIDTH; i++) {
      x = cx;
      y = cy;
      x2 = x * x;
      y2 = y * y;
      // compute the orbit z, f(z), f^2(z), f^3(z), ...
      // count the iterations until the orbit leaves the circle |z|=2.
      // stop if the number of iterations exceeds the bound MAX_ITERS.
      int n = 0;
      // >>>>>>>> CODE IS MISSING      
      while (n < MAX_ITERS && (x2 + y2) < 4.0) {
        // z^2 = (x + iy)^2 = (x^2 - y^2) + i(2xy)
        // z = z^2 + c = (x^2 - y^2 + cx) + i(2xy + cy)
        y = 2.0 * x * y + cy; // imaginary part
        x = x2 - y2 + cx; // real part
        x2 = x * x;
        y2 = y * y;
        n++;
      }
      nTotalIterationsCount += n;
      // >>>>>>>> CODE IS MISSING
      // n indicates if the point belongs to the mandelbrot set
      // plot the number of iterations at point (i, j)
      int c = ((long)n * 255) / MAX_ITERS;
      png_plot(pPng, i, j, c, c, c);
      cx += fDeltaX;
    }
  }
  double time_end = walltime();

  // print benchmark data
  printf("Total time:                 %g seconds\n",
         (time_end - time_start));
  printf("Image size:                 %ld x %ld = %ld Pixels\n",
         (long)IMAGE_WIDTH, (long)IMAGE_HEIGHT,
         (long)(IMAGE_WIDTH * IMAGE_HEIGHT));
  printf("Total number of iterations: %ld\n", nTotalIterationsCount);
  printf("Avg. time per pixel:        %g seconds\n",
         (time_end - time_start) / (double)(IMAGE_WIDTH * IMAGE_HEIGHT));
  printf("Avg. time per iteration:    %g seconds\n",
         (time_end - time_start) / (double)nTotalIterationsCount);
  printf("Iterations/second:          %g\n",
         nTotalIterationsCount / (time_end - time_start));
  // assume there are 8 floating point operations per iteration
  printf("MFlop/s:                    %g\n",
         nTotalIterationsCount * 8.0 / (time_end - time_start) * 1.e-6);
  
  // Output for gnuplot (format: num_threads time)
  printf("\n# Gnuplot data:\n");
  printf("%d %.6f\n", num_threads, (time_end - time_start));
  
  char filename[100];
  sprintf(filename, "mandel_%d.png", num_threads);

  png_write(pPng, filename); 
  return 0;
}
