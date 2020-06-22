/* Autor: Sérgio Rosendo da Silva Júnior */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "auxiliar_ep4.h"

int inInterval (int a, int b, int c)
{
    if ((a >= b) && (a <= c)) return (1);
    else return (0);
}

int isAlpha (char c)
{
    if ((inInterval (c, 65, 90)) || (inInterval (c, 97, 122)) || (inInterval (c, 192, 255))) return (1);
    else return (0);
}

int isAlphaNum (char c)
{
    if ((inInterval (c, 48, 57)) || (isAlpha (c))) return (1);
    else return (0);
}

char removeAccent (char c)
{
    if (!isAlpha(c)) return c;
    else if (inInterval (c, 192, 197) || inInterval (c, 224, 229)) return 'a';
    else if (c == 199 || c == 231) return 'c';
    else if (inInterval (c, 200, 203) || inInterval (c, 232, 235)) return 'e';
    else if (inInterval (c, 204, 207) || inInterval (c, 236, 239)) return 'i';
    else if (c == 209) return 'n';
    else if (inInterval (c, 210, 214) || inInterval (c, 242, 246)) return 'o';
    else if (inInterval (c, 217, 220)  || inInterval (c, 249, 252)) return 'u';
    else if (c == 221 || c == 253 || c == 255) return 'y';
    else return c;
}

void swap_symTable (symTable *s, symTable *t)
{
	symTable temp;

	temp = *s; *s = *t; *t = temp;
	return;
}

symTable *createVector_symTable (int n)
{
	symTable *tab;
	tab = malloc (n*sizeof (symTable));

	return tab;
}

void destroyVector_symTable (symTable *p)
{
    free (p);
    p = NULL;
    return;
}

symTable *resizeVector_symTable (symTable *v, int n)
{
    symTable *temp;
    temp = realloc (v, (n*sizeof (symTable)));

    return temp;
}

char *createVector_char (int n)
{
	char *c;
	c = malloc (n*sizeof (char));

	return c;
}

void destroyVector_char (char *c)
{
    free (c);
    c = NULL;
    return;
}

char *resizeVector_char (char *p, int n)
{
    char *temp;
    temp = realloc (p, (n*sizeof (char)));
    
    return temp;
}

cell *create_cell ()
{
    cell *temp;
    temp = malloc (sizeof (cell));

    return temp;
}

void destroy_cell (cell *c)
{
    free (c);
    c = NULL; 
    return;
}

node *create_node ()
{
    node *temp;
    temp = malloc (sizeof (node));

    return temp;
}

void destroy_node (node *c)
{
    free (c);
    c = NULL;
    return;
}

void destroy_tree (node *r)
{
   if (r != NULL) {
      destroy_tree (r->left);
      destroy_tree (r->right);
      destroy_node (r); 
   }    
}






    
        