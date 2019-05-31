#!/bin/sh

curl -LO http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27-win32-bin.zip
unzip tcc-0.9.27-win32-bin.zip
cp -n use-libtcc.cpp tcc
cp -n compile-link-run.bat tcc
echo Open a MSVC development terminal and execute compile-link-run.bat
