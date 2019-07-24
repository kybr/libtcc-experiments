#include <iostream>

#include "libtcc.h"

const char* code = R"(
void foo(double* d) {
  //*d = 2.1;
  d[0] = 2.1;
}
)";

int main(int argc, char* argv[]) {
  for (int k = 0; k < 10; k++) {
    TCCState* instance = tcc_new();
    tcc_set_output_type(instance, TCC_OUTPUT_MEMORY);
    tcc_compile_string(instance, code);
    // int size = tcc_relocate(instance, nullptr);
    tcc_relocate(instance, TCC_RELOCATE_AUTO);

    using Func = void (*)(double*);
    auto foo = (Func)tcc_get_symbol(instance, "foo");

    printf("stack single\n");
    {
      double d = 0;
      printf("d = %lf (before)\n", d);
      foo(&d);
      printf("d = %lf (after)\n", d);
    }

    printf("stack array\n");
    {
      double d[8];
      d[0] = 0;
      printf("d[0] = %lf (before)\n", d[0]);
      foo(d);
      printf("d[0] = %lf (after)\n", d[0]);
    }

    printf("heap array\n");
    {
      double* d = new double[8];
      d[0] = 0;
      printf("d[0] = %lf (before)\n", d[0]);
      foo(d);
      printf("d[0] = %lf (after)\n", d[0]);
      delete[] d;
    }

    tcc_delete(instance);
  }
  return 0;
}
