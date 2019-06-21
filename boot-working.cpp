#include "libtcc.h"

#include <cassert>
#include <iostream>

int main() {
  TCCState* instance = tcc_new();

  // i'm on Windows 10 x64. what else do i need here?
  //tcc_set_options(instance, "-shared");
  tcc_define_symbol(instance, "TCC_TARGET_X86_64", "");
  tcc_define_symbol(instance, "TCC_TARGET_PE", "");
  tcc_define_symbol(instance, "LIBTCC_AS_DLL", "");

  // i needed this define on Windows to get tcc_relocate in the DLL
  tcc_define_symbol(instance, "TCC_IS_NATIVE", "");

  tcc_set_output_type(instance, TCC_OUTPUT_MEMORY);

  // maybe add more source files?
  tcc_add_file(instance, "libtcc.c");

  int size = tcc_relocate(instance, (void*)0);
  void* memory = malloc(size);

  printf("need %d bytes\n", size);
  printf("relocating to %llx\n", (unsigned long long int)memory);
  tcc_relocate(instance, memory);

  // TCCState *tcc_new(void);
  using New = TCCState* (*)(void);
  New _new = (New)tcc_get_symbol(instance, "tcc_new");
  assert(_new != nullptr);

  // int tcc_set_output_type(TCCState *s, int output_type);
  using SetOutputType = int (*)(TCCState*, int);
  SetOutputType _set_output_type =
      (SetOutputType)tcc_get_symbol(instance, "tcc_set_output_type");
  assert(_set_output_type != nullptr);

  // int tcc_add_library_path(TCCState *s, const char *pathname);
  using AddLibraryPath = int(*)(TCCState*, const char*);
  AddLibraryPath _add_library_path = (AddLibraryPath)tcc_get_symbol(instance, "tcc_add_library_path");
  assert(_add_library_path != nullptr);

  // int tcc_compile_string(TCCState *s, const char *buf);
  using CompileString = int (*)(TCCState*, const char*);
  CompileString _compile_string =
      (CompileString)tcc_get_symbol(instance, "tcc_compile_string");
  assert(_compile_string != nullptr);

  // int tcc_relocate(TCCState *s1, void *ptr);
  using Relocate = int (*)(TCCState*, void*);
  Relocate _relocate = (Relocate)tcc_get_symbol(instance, "tcc_relocate");
  assert(_relocate != nullptr);

  // void *tcc_get_symbol(TCCState *s, const char *name);
  using GetSymbol = void* (*)(TCCState*, const char*);
  GetSymbol _get_symbol = (GetSymbol)tcc_get_symbol(instance, "tcc_get_symbol");
  assert(_get_symbol != nullptr);

  // can i delete the instance now??
  // my experiments suggest no.
  tcc_delete(instance);

  // use the new, relocated instance to compile something
  TCCState* a = _new();
  assert(a != nullptr);
  _set_output_type(a, TCC_OUTPUT_MEMORY);
  _add_library_path(a, "c:\\tcc\\lib");
  _compile_string(a, "int dec(int t) { return t - 1; }");
  _relocate(a, TCC_RELOCATE_AUTO);
  using Foo = int (*)(int);
  Foo dec = (Foo)_get_symbol(a, "dec");
  assert(dec != nullptr);
  printf("dec(2) == %d\b", dec(2));
}

