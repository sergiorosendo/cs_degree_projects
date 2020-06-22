/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _tabelaSimbolo_LD_h
#define _tabelaSimbolo_LD_h

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "auxiliar_ep4.h"
#include "tabelaSimbolo_LO.h"

int insert_LD (cell *head, cell *tail, char *pal, int len);
/* insere a string pal na lista com cabeça 'head' e rabo 'tail' ou computa uma 
nova ocorrencia caso a palavra ja esteja presente. Caso medio: O(n); */

void sort_symTable_L_A (cell *head, cell *tail, cell *l, cell *r);
/* ordena a lista com cabeça 'head' e rabo 'tail' em ordem alfabetica crescente
 de acordo com o conteudo da string de cada celula ('cell'->sT.word). 
 Caso medio: O(n log n); */

void sort_symTable_L_O (cell *head, cell *tail, cell *l, cell *r);
/* ordena a lista com cabeça 'head' e rabo 'tail' em ordem crescente do numero 
de ocorrencias ('cell'->sT.count). Caso medio: O(n log n); */

void sort_Symtable_L_O_A (cell *head, cell *tail);
/* ordena a lista com cabeça 'head' e rabo 'tail', já ordenada por numero de oco
rrencias, em ordem alfabetica mantendo o numero de ocorrencias como prioridade n
a ordenação. Caso medio: O(n log n); */

void tabelaSimbolo_L (char *fileName, int sorted, int alph);
/* imprime a tabela de simbolos do arquivo 'fileName' indicando o numero de oco
rrencias de cada palavra presente no mesmo. sorted != 0: utiliza lista orden
ada; sorted = 0 utiliza lista desordenada; alph != 0 impressao em ordem 
alfabetica; alph = 0: impressão em ordem de ocorrencias; */

#endif