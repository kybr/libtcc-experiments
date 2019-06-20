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


- - - 

- preparation:
  + install MSVC tools (choco install visualstudio2017-workload-vctools)
  + install git bash (choco install git)
  + make folders: "c:\tcc-cl" and "c:\tcc"
  + put both of those fodlers in the system PATH
- in GIT BASH:
  + git clone git://repo.or.cz/tinycc.git
  + edit tinycc/win32/build-tcc.bat adding to "build-tcc.bat"
    * echo>> ..\config.h #define TCC\_IS\_NATIVE
    * otherwise we don't get tcc\_relocate
    * see the example in this folder
- in Developer Command Prompt
  + cd tinycc/win32
  + build-tcc.bat -c cl -i c:\tcc-cl
  + build-tcc.bat -clean
  + confirm a working tcc in c:\tcc-cl by saying "tcc -vv"
  + build-tcc.bat -c tcc -i c:\tcc
  + build-tcc.bat -clean
  + confirm a working tcc in c:\tcc by saying "c:\tcc\tcc.exe -vv"
- REMOVE c:\tcc-cl from the system PATH
- in GIT BASH:
  + cd tinycc
  + tcc tests/libtcc\_test.c -I /c/tcc/libtcc /c/tcc/libtcc/libtcc.def 
  + libtcc\_test.exe

