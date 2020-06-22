/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _tabelaSimbolo_LO_h
#define _tabelaSimbolo_LO_h

#include "stdio.h"
#include "string.h"
#include "auxiliar_ep4.h"

int insert_cell (cell *x, cell *y, char *pal, int len);
/* insere uma nova celula contendo a string 'pal' em uma lista entre suas celul
as 'x' e 'y'. O(1)*/

int insert_LO (cell *head, cell *tail, char *pal, int len);
/* insere uma nova celula contendo a string 'pal' na lista com cabeça 'head' e 
rabo 'tail' ou contabiliza uma nova ocorrencia caso a palavra ja esteja present
e. Caso medio: O(n log n); */

#endif