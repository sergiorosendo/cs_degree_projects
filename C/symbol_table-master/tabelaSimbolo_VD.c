/* Autor: Sérgio Rosendo da Silva Júnior */
#include "tabelaSimbolo_VD.h"
#include "tabelaSimbolo_VO.h"

int insert_VD (symTable *v, int *n, char *pal, int len) 
{
    int i;

    if (pal[0] < 'n') {
        for (i = 0; i < *n; i++)
            if (strcmp (v[i].word, pal) == 0) {
                v[i].count++;
                return (1);
            }
    }
    else {
        for (i = (*n - 1); i >= 0; i--)
            if (strcmp (v[i].word, pal) == 0) {
                v[i].count++;
                return (1);
            }
    }

    return (insert (v, n, *n, pal, len));            
}

void sort_symTable_O (symTable *v, int l, int r)
{
    int pivo, flag = 0, beg, end;
    
    if (r - l <= 0) return;
    beg = l; end = r; pivo = r;
    while (r != l) {
        while (!flag && r != l)
            if (v[l].count > v[pivo].count) {
                swap_symTable (&v[pivo], &v[l]);
                pivo = l;
                flag = 1;
            }
            else l++;
        while (flag && r != l)
            if (v[r].count < v[pivo].count) {
                swap_symTable (&v[pivo], &v[r]);
                pivo = r;
                flag = 0;
            }
            else r--;
    }
    sort_symTable_O (v, beg, pivo - 1);
    sort_symTable_O (v, pivo + 1, end);
    return;
}

void sort_symTable_A (symTable *v, int l, int r)
{
    int pivo, flag = 0, beg, end;
    
    if (r - l <= 0) return;
    beg = l; end = r; pivo = r;
    while (r != l) {
        while (!flag && r != l)
            if (((strcmp (v[l].word, v[pivo].word)) > 0)) {
                swap_symTable (&v[pivo], &v[l]);
                pivo = l;
                flag = 1;
            }
            else l++;
        while (flag && r != l)
            if (((strcmp (v[r].word, v[pivo].word)) < 0)) {
                swap_symTable (&v[pivo], &v[r]);
                pivo = r;
                flag = 0;
            }
            else r--;
    }
    sort_symTable_A (v, beg, pivo - 1);
    sort_symTable_A (v, pivo + 1, end);
    return;
}

void sort_symtable_O_A (symTable *v, int n)
{
	int l = 0, r;

	while (l < n) {
		r = l;
		while ((v[r+1].count == v[l].count) && ((r + 1) < n)) r++;
		sort_symTable_A (v, l, r);
		l = r + 1;
	}
	return;
}

void tabelaSimbolo_V (char *fileName, int sorted, int alph)
{
    FILE *fp;
    char *pal, c;
    symTable *v;
    int i, n = 0, len = 0, maxLen = 50, maxTab = 100;
    
    fp = fopen (fileName, "r");
    if (!fp) {
        printf ("Nao foi possivel abrir o arquivo especificado.\n");
        return;
    } 
    pal = createVector_char (maxLen);
    v = createVector_symTable (maxTab);
    if (!pal || !v) goto error_noMem;

    while ((c = fgetc (fp)) != EOF) {
        if (isAlpha (c)) 
            while (isAlphaNum (c)) {
                /* encontrou o início de uma palavra */
                c = removeAccent (c);
                pal[len++] = tolower (c);
                c = fgetc (fp);
                if (len >= (maxLen - 2)) {
                    /* Palavra mais longa que o limite atual 'maxP' */
                    maxLen *=  2;
                    pal = resizeVector_char (pal, maxLen);
                    if (!pal) goto error_noMem;
                }
            }
        if (len > 0) {
            /* há uma palavra a ser adicionada à estrutura */
            pal[len++] = '\0'; 
            if (!sorted) { 
                if (!insert_VD (v, &n, pal, len)) goto error_noMem; 
            }
            else if (!insert_VO (v, &n, 0, n-1, pal, len)) goto error_noMem;
            len = 0;
            if (n >= (maxTab - 5)) { 
                /* Redimensiona o vetor v */
                maxTab *= 2;
                v = resizeVector_symTable (v, maxTab);
                if (!v) goto error_noMem;
            }            
        }
    }

    if (!alph) {
        sort_symTable_O (v, 0, n-1);
        sort_symtable_O_A (v, n);
    }
    else if (!sorted) sort_symTable_A (v, 0, n-1);
    
    for (i = 0; i < n; i++) printf ("%s %u\n", v[i].word, v[i].count);

    fclose (fp);
    destroyVector_char (pal);
    for (i = 0; i < n; i++) destroyVector_char (v[i].word);
    destroyVector_symTable (v);
    return;

    error_noMem:
        printf ("Memoria insuficiente.\n");
        fclose (fp);
        destroyVector_char (pal);
        for (i = 0; i < n; i++) destroyVector_char (v[i].word);
        destroyVector_symTable (v);
        return;
}