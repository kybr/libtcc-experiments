#include <iostream>
using std::cout;
using std::endl;

#include "libtcc.h"

int main(int argc, char* argv[]) {
  TCCState* instance = tcc_new();
  tcc_set_output_type(instance, TCC_OUTPUT_MEMORY);
  tcc_compile_string(instance, "char foo(int t) { return t; }");
  tcc_relocate(instance, TCC_RELOCATE_AUTO);
  char (*foo)(int) = (char (*)(int))tcc_get_symbol(instance, "foo");
  for (int t = 0; t < 10; t++)
    cout << (int)foo(t) << endl;
  tcc_delete(instance);
  return 0;
}
