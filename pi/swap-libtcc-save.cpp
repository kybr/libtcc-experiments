#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "libtcc.h"

using PlayFunc = float (*)(double);
using InitFunc = void (*)(void);

struct TCC {
  TCCState* instance{nullptr};
  PlayFunc function{nullptr};

  void maybe_destroy() {
    if (instance) {
      tcc_delete(instance);

      // tcc_delete does not null the pointer
      instance = nullptr;
    }
  }

  ~TCC() { maybe_destroy(); }

  int compile(const char* code) {
    maybe_destroy();
    instance = tcc_new();
    if (instance == nullptr) return 1;

    tcc_set_output_type(instance, TCC_OUTPUT_MEMORY);

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

struct SystemState {};

int main(int argc, char* argv[]) {
  TCC tcc[2];
  std::mutex lock;
  int active = 0;
  bool shouldSwap = false;
  bool done = false;

  std::thread audio([&]() {
    while (!done) {
      if (lock.try_lock()) {
        if (shouldSwap) active = 1 - active;
        shouldSwap = false;
        lock.unlock();
      }

      tcc[active](0);
    }
  });

  const int N = 1000000;

  std::chrono::high_resolution_clock::time_point then{
      std::chrono::high_resolution_clock::now()};

  for (int n = 0; n < N; n++) {
    lock.lock();
    if (tcc[1 - active].compile("float play(double t) { return 100 * t; }")) {
      return 1;
    }
    shouldSwap = true;
    lock.unlock();
    n++;
  }

  printf("trials: %d\n", N);
  printf("ellapsed: %lf\n",
         std::chrono::duration<double>(
             std::chrono::high_resolution_clock::now() - then)
             .count());

  done = true;
  audio.join();

  return 0;
}
