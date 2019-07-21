#include <chrono>
#include <iostream>

#include "libtcc.h"

int main(int argc, char* argv[]) {
  const int N = 200000 * 10;  // ~10 minutes
  double sum = 0;
  double minimum = 9999e9;
  double maximum = 0;
  std::chrono::high_resolution_clock::time_point then{
      std::chrono::high_resolution_clock::now()};

  for (int n = 0; n < N; n++) {
    std::chrono::high_resolution_clock::time_point begin{
        std::chrono::high_resolution_clock::now()};

    {
      TCCState* instance = tcc_new();
      tcc_set_output_type(instance, TCC_OUTPUT_MEMORY);
      tcc_compile_string(instance, "char foo(int t) { return t; }");
      tcc_relocate(instance, TCC_RELOCATE_AUTO);
      char (*foo)(int) = (char (*)(int))tcc_get_symbol(instance, "foo");
      char c = foo(0);
      tcc_delete(instance);
    }

    double t = std::chrono::duration<double>(
                   std::chrono::high_resolution_clock::now() - begin)
                   .count();

    if (t < minimum) minimum = t;
    if (t > maximum) maximum = t;
    sum += t;
  }

  printf("trials: %d\n", N);
  printf("minimum: %lf\n", minimum);
  printf("mean: %lf\n", sum / N);
  printf("maximum: %lf\n", maximum);
  printf("ellapsed: %lf\n",
         std::chrono::duration<double>(
             std::chrono::high_resolution_clock::now() - then)
             .count());

  return 0;
}

// Raspberry Pi 4b (2GB)
//
// trials: 1000000
// minimum: 0.000280
// mean: 0.000295
// maximum: 0.001452
// ellapsed: 296.354510
//
// trials: 2000000
// minimum: 0.000273
// mean: 0.000283
// maximum: 0.002978
// ellapsed: 567.364519
