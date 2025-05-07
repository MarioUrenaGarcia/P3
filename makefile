#Este es el archivo de makefile para kubo.c

kubo.exe: kubo.o funKubo.o funArbol.o
	gcc kubo.o funKubo.o funArbol.o -o kubo.exe

funArbol.o: funArbol.c defKubo.h
	gcc -c funArbol.c

funKubo.o: funKubo.c defKubo.h
	gcc -c funKubo.c

kubo.o: kubo.c defKubo.h
	gcc -c kubo.c