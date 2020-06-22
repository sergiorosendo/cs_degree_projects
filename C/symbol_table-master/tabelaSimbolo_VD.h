/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _tabelaSimbolo_VD_h
#define _tabelaSimbolo_VD_h

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "auxiliar_ep4.h"

int insert_VD (symTable *v, int *n, char *pal, int len);
/* insere a string 'pal' de comprimento 'len' no vetor v e acrescenta 1 a n ou 
apenas contabiliza mais uma ocorrencia caso a mesma exista no vetor. Caso 
medio: O(n); */

void sort_symTable_O (symTable *v, int l, int r);
/* ordena os elementos do vetor 'v[l...r]' de acordo com o numero de 
ocorrencias (v[i].count). Caso medio: O(n log n);*/

void sort_symTable_A (symTable *v, int l, int r);    
/* ordena os elementos do vetor 'v[l...r]' em ordem alfabetica de suas strings
(v[i].sT.word). Caso medio: O(n log n);*/

void sort_symtable_O_A (symTable *v, int n);
/* ordena o vetor v[0...(n-1)], já ordenado por numero de ocorrencias, em 
ordem alfabetica mantendo o numero de ocorrencias como prioridade na ordenação.
 Caso medio: O(n log n); */

void tabelaSimbolo_V (char *fileName, int sorted, int alph);
/* imprime a tabela de simbolos do arquivo 'fileName' indicando o numero de oco
rrencias de cada palavra presente no mesmo. sorted != 0: utiliza vetor ordenado
; sorted = 0 utiliza vetor desordenado; alph != 0 impressao em ordem alfabetica; 
alph = 0: impressão em ordem de ocorrencias; Caso medio: O();*/ 

#endif