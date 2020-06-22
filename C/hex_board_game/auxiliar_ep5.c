/* Autor: Sérgio Rosendo da Silva Júnior */
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "auxiliar_ep5.h"

int inInterval (int a, int b, int c)
{
    if ((a >= b) && (a <= c)) return (1);
    else return (0);
}

int char_inArray (char c, char *v, int n)
{
    int i;

    for (i = 0; i < n; i++) 
        if (v[i] == c) return (1);

    return (0);
}

void swap_char (char *a, char *b)
{
    char temp;
    temp = *a; *a = *b; *b = temp;
    return;
}

char *createVector_char (int n)
{
    char *c;
    c = malloc (n*sizeof (char));

    return c;
}

void destroyVector_char (char *c)
{
    free (c); c = NULL; 
    return;
}

char *resizeVector_char (char *p, int n)
{
    char *temp;
    temp = realloc (p, (n*sizeof (char)));

    return temp;
}

char **createMatrix_char (int lines, int columns)
{
    int i; char **m;
    m = malloc (lines*sizeof (char *));
    if (!m) return NULL;
    for (i = 0; i < lines; i++) {
        m[i] = createVector_char (columns);
        if (!m[i]) 
            while (i >= 0) 
                destroyVector_char (m[i--]);
    }

    return m;
}

void destroyMatrix_char (char **m, int lines)
{
    int i;
    for (i = 0; i < lines; i++) 
        destroyVector_char (m[i]);
    
    free (m); m = NULL; 
    
    return;
}

int *createVector_int (int n)
{
    int *v;
    v = malloc (n*sizeof (int));

    return v;
}

void destroyVector_int (int *c)
{
    free (c); c = NULL; 
    return;
}

int *resizeVector_int (int *p, int n)
{
    int *temp;
    temp = realloc (p, (n*sizeof (int)));

    return temp;
}

int **createMatrix_int (int lines, int columns)
{
    int i, **m;
    m = malloc (lines*sizeof (int *));
    if (!m) return NULL;
    for (i = 0; i < lines; i++) {
        m[i] = createVector_int (columns);
        if (!m[i]) 
            while (i >= 0) 
                destroyVector_int (m[i--]);
    }

    return m;
}

void destroyMatrix_int (int **m, int lines)
{
    int i;
    for (i = 0; i < lines; i++) 
        destroyVector_int (m[i]);
    
    free (m); m = NULL; 
    
    return;
}