#include <chrono>
#include <iostream>
#include <mutex>

#include "libtcc.h"

using PlayFunc = float (*)(double);
using InitFunc = void (*)(void);

struct TCC {
  TCCState* instance{nullptr};
  PlayFunc function{nullptr};

  void maybe_destroy() {
    if (instance) {
      tcc_delete(instance);
      instance = nullptr;
    }
  }

  ~TCC() { maybe_destroy(); }

  int compile(const char* code) {
    maybe_destroy();
    instance = tcc_new();
    if (instance == nullptr) return 1;
    tcc_set_output_type(instance, TCC_OUTPUT_MEMORY);
    // tcc_enable_debug(instance);
    if (0 != tcc_compile_string(instance, code)) return 2;
    if (-1 == tcc_relocate(instance, TCC_RELOCATE_AUTO)) return 3;
    function = (PlayFunc)tcc_get_symbol(instance, "play");
    if (function == nullptr) return 4;
    InitFunc init = (InitFunc)tcc_get_symbol(instance, "init");
    if (init) init();
    return 0;
  }

  float operator()(double t) {
    if (function)
      return function(t);
    else
      return 0;
  }
};

int main(int argc, char* argv[]) {
  const int N = 200000 * 1;
  double sum = 0;
  double minimum = 9999e9;
  double maximum = 0;
  std::chrono::high_resolution_clock::time_point then{
      std::chrono::high_resolution_clock::now()};

  for (int n = 0; n < N; n++) {
    std::chrono::high_resolution_clock::time_point begin{
        std::chrono::high_resolution_clock::now()};

    {
      TCC tcc;

      if (tcc.compile("float play(double t) { return 100 * t; }")) {
        n = N;
      } else {
        tcc(0);
      }
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
