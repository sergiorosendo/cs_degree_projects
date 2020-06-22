/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _tabelaSimbolo_VO_h
#define _tabelaSimbolo_VO_h

#include "stdio.h"
#include "string.h"
#include "auxiliar_ep4.h"

int insert (symTable *v, int *n, int pos, char *pal, int len);
/* insere a string 'pal' de comprimento 'len' no vetor 'v' tal que 'v[pos].word = pal' e 
'v[pos].count = 1' e acrescenta 1 a '*n'. O vetor precisa ter espaço para a inserção de um novo 
elemento. Caso medio: O(1); */

int insert_VO (symTable *v, int *n, int beg, int end, char *pal, int len);
/* insere a string 'pal' de comprimento 'len' no vetor 'v[beg...end]'' ou computa 
mais uma ocorrencia caso a mesma ja esteja presente no vetor. Caso medio: O(log n); */

#endif