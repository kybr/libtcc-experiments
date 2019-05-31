@echo off

echo ####################################################
rem del exports.txt libtcc.lib

dumpbin /exports libtcc.dll > exports.txt

echo LIBRARY libtcc.dll > libtcc.def
echo EXPORTS >> libtcc.def
for /f "skip=19 tokens=1,4" %%A in (exports.txt) do if NOT "%%B" == "" (echo %%B @%%A >> libtcc.def)

lib /def:libtcc.def /out:libtcc.lib /machine:x64

rem del libtcc.def libtcc.exp exports.txt


REM me..

REM set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.16.27023\include;C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt;C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\shared;C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\winrt;C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\cppwinrt

REM set LIB=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.16.27023\lib\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.17763.0\ucrt\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.17763.0\um\x86;

REM set LIB=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.16.27023\lib\x86;C:\Program Files (x86)\Windows Kits\10\lib\10.0.17763.0\ucrt\x86;C:\Program Files (x86)\Windows Kits\10\lib\10.0.17763.0\um\x86;

echo ####################################################
rem del use-libtcc.obj
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\vc\Tools\MSVC\14.16.27023\bin\Hostx64\x64\cl.exe" /c use-libtcc.cpp /I libtcc /clr

echo ####################################################
rem del use-libtcc.exe
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\vc\Tools\MSVC\14.16.27023\bin\Hostx64\x64\link.exe" /machine:x64 use-libtcc.obj libtcc.lib
