# Autor: Sérgio Rosendo da Silva Júnior
CC = gcc

CFLAGS = -c -Wall -ansi -pedantic -O2 -g 

all: ep5

ep5: hex.o ai_strategy.o board_management.o auxiliar_ep5.o 
	$(CC) hex.o ai_strategy.o board_management.o auxiliar_ep5.o -lm -o ep5

hex.o: hex.c
	$(CC) $(CFLAGS) hex.c

ai_strategy.o: ai_strategy.c
	$(CC) $(CFLAGS) ai_strategy.c 

board_management.o: board_management.c
	$(CC) $(CFLAGS) board_management.c

auxiliar_ep5.o: auxiliar_ep5.c
	$(CC) $(CFLAGS) auxiliar_ep5.c

clean:
	rm -rf *o ep5