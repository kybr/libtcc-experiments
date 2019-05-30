use-libtcc.exe: use-libtcc.c
	./tcc/tcc.exe use-libtcc.c tcc/libtcc.dll -I tcc/libtcc
	./use-libtcc.exe

use-tcc.exe: use-tcc.c
	./tcc/tcc.exe use-tcc.c 
	./use-tcc.exe
