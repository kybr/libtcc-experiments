# Experiments with the Tiny C Compiler

Currently, we are working on how to build against libtcc on Windows.

## Installation

To "install" the latest Tiny C Compiler on Windows, we build from source:

**Preparation:**
- Install MSVC tools (`choco install visualstudio2017-workload-vctools`)
- Install git bash (`choco install git`)
- Make folders: `c:\tcc-cl` and `c:\tcc`
- Put both of those folders in the system PATH

**In GIT BASH:**
- `git clone git://repo.or.cz/tinycc.git`
- Edit `tinycc/win32/build-tcc.bat` adding `echo>> ..\config.h #define TCC_IS_NATIVE`
  + otherwise we don't get `tcc_relocate` in the DLL!!
  + see the example `build-tcc.bat` in this folder for details

**In Developer Command Prompt:**
```
cd tinycc/win32
build-tcc.bat -c cl -i c:\tcc-cl
build-tcc.bat -clean
tcc -vv                           :: confirm a working tcc in c:\tcc-cl
build-tcc.bat -c tcc -i c:\tcc
build-tcc.bat -clean
c:\tcc\tcc.exe -vv                :: confirm a working tcc in c:\tcc
```

**Remove `c:\tcc-cl` from the system PATH**

**In GIT BASH:**
```
cd tinycc
tcc tests/libtcc_test.c -I /c/tcc/libtcc /c/tcc/libtcc/libtcc.def 
libtcc_test.exe
```

## Build against TCC

1. Download the win32 version of TCC from here: <http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27-win32-bin.zip>
2. Expand the .zip somewhere (call it X)
3. Open a MSVC development terminal and navigate to X
4. Copy `use-libtcc.cpp` from this repository to X
5. From X, run these MSVC commands:
  + `lib /def:libtcc\libtcc.def /out:libtcc.lib`
  + `cl /MD use-libtcc.cpp -I libtcc libtcc.lib`
  + `use-libtcc.exe`

The first command makes a .lib from the libtcc.def and libtcc.dll files. The second command compiles and links.

