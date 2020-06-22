/* Autor: Sérgio Rosendo da Silva Júnior */
#include "tabelaSimbolo_LD.h"
#include "tabelaSimbolo_LO.h"

int insert_LD (cell *head, cell *tail, char *pal, int len)
{
    cell *c;
    
    if (pal[0] < 'n') {
        c = head->next;
        while (c != tail) {
            if (strcmp (c->sT.word, pal) == 0) {
                c->sT.count++;
                return (1);
            } 
            else c = c->next;
        }
    }
    else {
        c = tail->prev;
        while (c != head) {
            if (strcmp (c->sT.word, pal) == 0) {
                c->sT.count++; 
                return (1);
            } 
            else c = c->prev;
        }
    }

    return (insert_cell (tail->prev, tail, pal, len));
}

void sort_symTable_L_A (cell *head, cell *tail, cell *l, cell *r)
{
    int flag = 0;
    cell *beg, *end, *pivo;

    if (l == tail || r == head) return;
    if (l == head || r == tail) return;
    if (r == l) return;
	beg = l; end = r; pivo = r;

    while (r != l) {
        while (!flag && r != l)
            if (strcmp (l->sT.word, pivo->sT.word) > 0) {
                swap_symTable (&(pivo->sT), &(l->sT));
                pivo = l;
                flag = 1;
            }
            else l = l->next;
        while (flag && r != l)
            if (strcmp (r->sT.word, pivo->sT.word) < 0) {
                swap_symTable (&(pivo->sT), &(r->sT));
                pivo = r;
                flag = 0;
            }
            else r = r->prev;
    }

    sort_symTable_L_A (head, pivo, beg, pivo->prev);
    sort_symTable_L_A (pivo, tail, pivo->next, end);
    return;
}

void sort_symTable_L_O (cell *head, cell *tail, cell *l, cell *r)
{
    int flag = 0;
    cell *beg, *end, *pivo;

    if (l == tail || r == head) return;
    if (l == head || r == tail) return;
    if (r == l) return;
	beg = l; end = r; pivo = r;

    while (r != l) {
        while (!flag && r != l)
            if ((l->sT.count) > (pivo->sT.count)) {
                swap_symTable (&(pivo->sT), &(l->sT));
                pivo = l;
                flag = 1;
            }
            else l = l->next;
        while (flag && r != l)
            if ((r->sT.count) < (pivo->sT.count)) {
                swap_symTable (&(pivo->sT), &(r->sT));
                pivo = r;
                flag = 0;
            }
            else r = r->prev;
    }

    sort_symTable_L_O (head, pivo, beg, pivo->prev);
    sort_symTable_L_O (pivo, tail, pivo->next, end);
    return;
}

void sort_Symtable_L_O_A (cell *head, cell *tail)
{
	cell *l, *r;

	l = head->next;

	while (l != tail) {
		r = l;
		while ((r->next != tail) && ((r->next)->sT.count == l->sT.count)) r = r->next;
		sort_symTable_L_A (head, r->next, l, r);
		l = r->next;
	}
	return;
}

void tabelaSimbolo_L (char *fileName, int sorted, int alph)
{
    FILE *fp;
    char *pal, c;
    cell *head, *tail, *C, *temp;
    int len = 0, maxLen = 50;
    
    fp = fopen (fileName, "r");
    if (!fp) {
        printf ("Nao foi possivel abrir o arquivo especificado.\n");
        return;
    } 
    pal = createVector_char (maxLen);
    head = create_cell (); 
    tail = create_cell ();
    if (!pal || !head || !tail) goto error_noMem;

    head->next = tail; head->prev = NULL;
    tail->next = NULL; tail->prev = head;

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
                if (!insert_LD (head, tail, pal, len)) goto error_noMem;
            }
            else if (!insert_LO (head, tail, pal, len)) goto error_noMem;
            len = 0;     
        }
    }

    if (!alph) {
        sort_symTable_L_O (head, tail, head->next, tail->prev);
        sort_Symtable_L_O_A (head, tail);
    }
    else if (!sorted) sort_symTable_L_A (head, tail, head->next, tail->prev);
    
    for (C = head->next; C != tail; C = C->next)
        printf ("%s %u\n", C->sT.word, C->sT.count);

    fclose (fp);
    destroyVector_char (pal);
    for (C = head; C; C = temp) { 
        temp = C->next;
        if (C != head && C != tail) destroyVector_char (C->sT.word);
        destroy_cell (C);
    }
    return;

    error_noMem:
        printf ("Memoria insuficiente.\n");
        fclose (fp);
        destroyVector_char (pal);
        for (C = head; C; C = temp) { 
            temp = C->next;
            if (C != head && C != tail) destroyVector_char (C->sT.word);
            destroy_cell (C);
        }
        return;
}
