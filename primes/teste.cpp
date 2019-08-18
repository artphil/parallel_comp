#include <omp.h>
#include <math.h>
#include <time.h>
#include <iostream>

int main(int argc, char *argv[])
{
  int i, nthreads;
  clock_t clock_timer;
  double wall_timer;
  int max = 1000000;
  double c[max];
  for (nthreads = 1; nthreads <= 8; ++nthreads)
  {
    clock_timer = clock();
    wall_timer = omp_get_wtime();
#pragma omp parallel for private(i) num_threads(nthreads)
    for (i = 0; i < max; i++)
      c[i] = sqrt(i * 4 + i * 2 + i);
    std::cout << "threads: " << nthreads << " time on clock(): " << (double)(clock() - clock_timer) / CLOCKS_PER_SEC
              << " time on wall: " << omp_get_wtime() - wall_timer << "\n";
  }
}