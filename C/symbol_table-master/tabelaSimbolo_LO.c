/* Autor: Sérgio Rosendo da Silva Júnior */
#include "tabelaSimbolo_LO.h"

int insert_cell (cell *x, cell *y, char *pal, int len)
{
    cell *new;
    char *P;

    P = createVector_char (len);
    if (!P) return (0);
    strcpy (P, pal);

    new = create_cell ();
    if (!new) return (0);

    new->sT.word = P;
    new->sT.count = 1;

    new->next = y;
    new->prev = x;
    x->next = new;
    y->prev = new;

    return (1);
}

int insert_LO (cell *head, cell *tail, char *pal, int len)
{
    cell *c;
    c = head->next;

    if (c == tail) return (insert_cell (c->prev, c, pal, len));

    while (c != tail)
        if (strcmp (c->sT.word, pal) == 0) {
            c->sT.count++;
            return (1);
        }
        else if ((strcmp (c->sT.word, pal) < 0) && ((c->next == tail) || (strcmp ((c->next)->sT.word, pal) > 0)))
            return (insert_cell (c, c->next, pal, len));
        else if ((strcmp (c->sT.word, pal) > 0) && ((c->prev == head) || (strcmp ((c->prev)->sT.word, pal) < 0)))
            return (insert_cell (c->prev, c, pal, len));
        else c = c->next;

    return (1);
}