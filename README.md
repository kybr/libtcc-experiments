# Experiments with the Tiny C Compiler

Currently, we are working on how to build against libtcc on Windows.

See `build-on-windows`.


1. Download the win32 version of TCC from here: <http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27-win32-bin.zip>
2. Expand the .zip somewhere (call it X)
3. Open a MSVC development terminal and navigate to X
4. Copy `use-libtcc.cpp` from this repository to X
5. From X, run these MSVC commands:
  + `lib /def:libtcc\libtcc.def /out:libtcc.lib`
  + `cl /MD use-libtcc.cpp -I libtcc libtcc.lib`
  + `use-libtcc.exe`

The first command makes a .lib from the libtcc.def and libtcc.dll files. The second command compiles and links.
