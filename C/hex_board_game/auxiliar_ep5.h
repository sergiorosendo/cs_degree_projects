/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _auxiliar_ep5_h
#define _auxiliar_ep5_h

int inInterval (int a, int b, int c);
/* Retorna (1) se um inteiro 'a' esta no intervalo [b, c] e (0) caso contrario. */

int char_inArray (char c, char *v, int n);
/* Retorna (1) se o vetor v[n] contém c e (0) caso contrario. */

void swap_char (char *a, char *b);
/* Troca o conteúdo de *a com *b. */

char *createVector_char (int n);
/* Aloca memoria para um vetor de caracteres [n] e retorna o ponteiro correspondente ou 'NULL' caso tenha falhado. */

char *resizeVector_char (char *p, int n);
/* Realoca '(n*sizeof (char))' bytes com o mesmo conteudo do vetor 'p[]' de 'p[0]' até 'p[n-1]'. Libera a memoria 
originalmente alocada para v e retorna o novo ponteiro ou 'NULL' caso tenha falhado. */

void destroyVector_char (char *c);
/* Libera a memoria alocada a 'c' */

char **createMatrix_char (int lines, int columns);
/* Aloca memoria para uma matriz m[lines][columns] e retorna o ponteiro correspondente ou 'NULL' caso tenha falhado. */

void destroyMatrix_char (char **m, int lines);
/* Libera a memoria alocada para a matriz 'm' com 'lin' linhas. */

int *createVector_int (int n);
/* Aloca memoria para um vetor de inteiros [n] e retorna o ponteiro correspondente ou 'NULL' caso tenha falhado. */

int *resizeVector_int (int *p, int n);
/* Realoca '(n*sizeof (int))' bytes com o mesmo conteudo do vetor 'p[]' de 'p[0]' até 'p[n-1]'. Libera a memoria 
originalmente alocada para v e retorna o novo ponteiro ou 'NULL' caso tenha falhado. */

void destroyVector_int (int *c);
/* Libera a memoria alocada a 'c' */

int **createMatrix_int (int lines, int columns);
/* Aloca memoria para uma matriz m[lines][columns] e retorna o ponteiro correspondente ou 'NULL' caso tenha falhado. */

void destroyMatrix_int (int **m, int lines);
/* Libera a memoria alocada para a matriz 'm' com 'lin' linhas. */

#endif