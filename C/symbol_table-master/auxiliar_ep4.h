/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _auxiliar_ep4_h
#define _auxiliar_ep4_h

typedef struct symT {
    char *word;
    unsigned int count;
} symTable;

typedef struct list{
	struct symT sT;
	struct list *next;
	struct list *prev;
} cell;

typedef struct tree {
	struct symT sT;
	struct tree *left;
	struct tree *right;
	struct tree *parent;
} node;

int inInterval (int a, int b, int c);
/* Retorna 1 se um inteiro 'a' esta no intervalo [b, c] e 0 caso contrario. */

int isAlpha (char c);
/* Retorna 1 se o caractere 'c' e uma letra. */

int isAlphaNum (char c);
/* Retorna 1 se o caractere 'c' e alfa numerico. */

char removeAccent (char c);
/* Retorna, se existir, o caractere sem acento corresponde a 'c'.
Se não existir, retorna 'c'. */

void swap_symTable (symTable *s, symTable *t);
/* Troca o conteúdo de s com t. */

symTable *createVector_symTable (int n);
/* Aloca '(n*sizeof (symTable))' bytes, retorna o ponteiro correspondente ou 
'NULL' caso tenha falhado. */

symTable *resizeVector_symTable (symTable *v, int n);
/* Realoca '(n*sizeof (symTable))' bytes com o mesmo conteudo do vetor 'v[]' 
de 'v[0]' até 'v[n-1]'. Libera a memoria originalmente alocada para v e reto
rna o novo ponteiro ou 'NULL' caso tenha falhado. */

void destroyVector_symTable (symTable *p);
/* Libera a memoria alocada a 'p'. */

char *createVector_char (int n);
/* Aloca '(n*sizeof (char))' bytes, retorna o ponteiro correspondente ou 
'NULL' caso tenha falhado. */

char *resizeVector_char (char *p, int n);
/* Realoca '(n*sizeof (char))' bytes com o mesmo conteudo do vetor 'v[]' 
de 'v[0]' até 'v[n-1]'. Libera a memoria originalmente alocada para v e reto
rna o novo ponteiro ou 'NULL' caso tenha falhado. */

void destroyVector_char (char *c);
/* Libera a memoria alocada a 'c' */

cell *create_cell ();
/* Aloca '(sizeof (cell))' bytes, retorna o ponteiro correspondente ou 
'NULL' caso tenha falhado. */

void destroy_cell (cell *c);
/* Libera a memoria alocada a 'c'. */

node *create_node ();
/* Aloca '(sizeof (node))' bytes, retorna o ponteiro correspondente ou 
'NULL' caso tenha falhado. */

void destroy_node (node *c);
/* Libera a memoria alocada a 'c'. */

void destroy_tree (node *r);
/* Libera a memoria alocada a arvore binaria com raiz 'r'. */

#endif