/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _tabelaSimbolo_AB_h
#define _tabelaSimbolo_AB_h

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "auxiliar_ep4.h"
#include "tabelaSimbolo_VD.h"

int insert_AB (node *root, int *n, char *pal, int len);
/* insere a string 'pal' na arvore de busca binaria de raiz 'root'ou computa 1 nova ocor
rencia caso a palavra ja esteja presente. Caso medio: O(log(n)); */

void print_lrr (node *r);
/* imprime o conteudo da arvore binaria de raiz 'r' seguindo a varredura e-d-r. 
Complexidade: O(n); */

void transferTo_vector (symTable *v, node *r, int *i);
/* transfere o conteudo de uma arvore de busca binaria de raiz 'r' para o vetor
 'v' seguindo a varredura e-d-r. Complexidade: O(n) */

void tabelaSimbolo_AB (char *fileName, int alph);
/* imprime a tabela de simbolos do arquivo 'fileName' indicando o numero de oco
rrencias de cada palavra presente no mesmo. alph = 0: impressão em ordem de oco
rrencias; alph != 0 impressao em ordem alfabetica; */

#endif