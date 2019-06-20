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

(Find logs of these builds in `build-log-tcc-cl.txt` and `build-log-tcc.txt`.)


**In Developer Command Prompt:** (optional)

```
cd c:\tcc                         
lib /machine:x86 /def:libtcc\libtcc.def /out:libtcc.lib
lib /machine:x64 /def:libtcc\libtcc.def /out:libtcc64.lib
cd c:\tcc-cl
lib /machine:x86 /def:libtcc\libtcc.def /out:libtcc.lib
lib /machine:x64 /def:libtcc\libtcc.def /out:libtcc64.lib
```

(The stuff above is necessary if we later want to use cl.exe to build against libtcc.)


**Remove `c:\tcc-cl` from the system PATH**

**In GIT BASH:**
```
cd tinycc
tcc tests/libtcc_test.c -I /c/tcc/libtcc /c/tcc/libtcc/libtcc.def 
libtcc_test.exe
```




## Build against TCC

Use the x64 Developer Command Prompt. Start a cmd.exe and do:

%comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

```
cl /MD use-libtcc.cpp /I c:\tcc\libtcc c:\tcc\libtcc64.lib
use-libtcc.exe
```

