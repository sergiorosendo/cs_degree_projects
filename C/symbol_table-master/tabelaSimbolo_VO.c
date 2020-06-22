/* Autor: Sérgio Rosendo da Silva Júnior */
#include "tabelaSimbolo_VO.h"

int insert (symTable v[], int *n, int pos, char *pal, int len)
{
    char *P;
    int i;
    
    P = createVector_char (len);
    if (!P) return (0);
    strcpy (P, pal);

    for (i = *n; i > pos; i--) v[i] = v[i-1];
    
    v[pos].word = P;
    v[pos].count = 1;
    ++*n;

    return (1);
}

int insert_VO (symTable v[], int *n, int beg, int end, char *pal, int len)
{
    /* Insere a string 'pal' no vetor ordenado de tabela de simbolos; */
    int mid;

    if (!*n) {
    	insert (v, n, 0, pal, len);
    	return (1);
    }
    else if (beg > end) return -1;
    mid = (beg + end)/2;

    if (strcmp (v[mid].word, pal) == 0) {
    	/* 'pal' já existe na tabela */
        v[mid].count++;
    	return (1);
    }
    else if (strcmp (v[mid].word, pal) > 0)
    	if ((mid == 0) || (strcmp (v[mid-1].word, pal) < 0))
    		/* 'pal' não está presente na tabela */
    		return (insert (v, n, mid, pal, len));
    	else
    		return (insert_VO (v, n, beg, mid - 1, pal, len));
    else
    	if (((mid + 1) > (*n - 1)) || (strcmp (v[mid+1].word, pal) > 0))
    		/* 'pal' não está presente na tabela */
    		return (insert (v, n, mid + 1, pal, len));
    	else
        	return (insert_VO (v, n, mid + 1, end, pal, len));
}