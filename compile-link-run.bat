lib /def:libtcc\libtcc.def /out:libtcc.lib
cl /MD use-libtcc.cpp -I libtcc libtcc.lib
use-libtcc.exe
