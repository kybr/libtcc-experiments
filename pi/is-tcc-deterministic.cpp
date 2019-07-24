#include <iostream>
using std::cout;
using std::endl;

#include "libtcc.h"

const char* code = R"(
float foo(double d) {
  return 10.1 * d * d - 100.1 * d + 1;
}
)";

int main(int argc, char* argv[]) {
  for (int k = 0; k < 10; k++) {
    TCCState* instance = tcc_new();
    tcc_set_output_type(instance, TCC_OUTPUT_MEMORY);
    tcc_compile_string(instance, code);
    int size = tcc_relocate(instance, nullptr);
    char* p = new char[size];
    tcc_relocate(instance, p);

    for (int i = 0; i < size; i++) {
      // if (i % 16 == 0) printf("\n");
      printf("%02zx", p[i]);
    }
    printf("\n");

    // tcc_relocate(instance, TCC_RELOCATE_AUTO);
    using Func = float (*)(double);
    auto foo = (Func)tcc_get_symbol(instance, "foo");
    for (double t = 1; t < 10; t++) foo(1 / t);
    // for (double t = 1; t < 10; t++) cout << foo(1 / t) << endl;
    tcc_delete(instance);

    delete[] p;
  }
  return 0;
}
