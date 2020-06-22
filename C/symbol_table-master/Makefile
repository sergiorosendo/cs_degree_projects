# Autor: Sérgio Rosendo da Silva Júnior
CC = gcc

CFLAGS = -c -Wall -ansi -pedantic -O2 -g

all: ep4

ep4: listaSimbolo.o auxiliar_ep4.o tabelaSimbolo_AB.o tabelaSimbolo_LD.o tabelaSimbolo_LO.o tabelaSimbolo_VD.o tabelaSimbolo_VO.o
	$(CC) listaSimbolo.o auxiliar_ep4.o tabelaSimbolo_AB.o tabelaSimbolo_LD.o tabelaSimbolo_LO.o tabelaSimbolo_VD.o tabelaSimbolo_VO.o -o ep4

auxiliar_ep4.o: auxiliar_ep4.c
	$(CC) $(CFLAGS) auxiliar_ep4.c

listaSimbolo.o: listaSimbolo.c
	$(CC) $(CFLAGS) listaSimbolo.c

tabelaSimbolo_AB.o: tabelaSimbolo_AB.c
	$(CC) $(CFLAGS) tabelaSimbolo_AB.c

tabelaSimbolo_LD.o: tabelaSimbolo_LD.c
	$(CC) $(CFLAGS) tabelaSimbolo_LD.c

tabelaSimbolo_LO.o: tabelaSimbolo_LO.c
	$(CC) $(CFLAGS) tabelaSimbolo_LO.c

tabelaSimbolo_VD.o: tabelaSimbolo_VD.c
	$(CC) $(CFLAGS) tabelaSimbolo_VD.c

tabelaSimbolo_VO.o: tabelaSimbolo_VO.c
	$(CC) $(CFLAGS) tabelaSimbolo_VO.c

clean:
	rm -rf *o ep4