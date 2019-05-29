use-libtcc.exe: use-libtcc.c
	@./tcc.exe use-libtcc.c libtcc.dll -I libtcc
	./use-libtcc.exe

use-tcc.exe: use-tcc.c
	@./tcc.exe use-tcc.c 
	./use-tcc.exe
