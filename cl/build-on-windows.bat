@echo off
curl -LO http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27-win32-bin.zip
PowerShell Expand-Archive -Path "tcc-0.9.27-win32-bin.zip" -DestinationPath "tcc"
copy /y use-libtcc.cpp tcc
cd tcc
lib /def:libtcc\libtcc.def /out:libtcc.lib
cl /MD use-libtcc.cpp -I libtcc libtcc.lib
use-libtcc.exe
