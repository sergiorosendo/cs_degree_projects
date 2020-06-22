/* Autor: Sérgio Rosendo da Silva Júnior */
#include "tabelaSimbolo_AB.h"

void print_lrr (node *r) 
{
   if (r != NULL) {
      print_lrr (r->left);
      printf ("%s %u\n", r->sT.word, r->sT.count);
      print_lrr (r->right);
   }
}

int insert_AB (node *root, int *n, char *pal, int len) 
{
	int Left = 1, flag = 0;
    char *P;
    node *new, *current;

    if (*n == 0) {
        P = createVector_char (len);
        if (!P) return (0);
        strcpy (P, pal);

    	root->sT.word = P;
    	root->sT.count = 1;
        ++*n;
    	return (1);
    }

    current = root;
	while (!flag) {
	    if (strcmp (current->sT.word, pal) == 0) {
	    	current->sT.count++;
	    	return (1);
	    }
		else if (strcmp (current->sT.word, pal) > 0) {
			if (!(current->left)) flag = 1;
			else current = current->left;
		}
		else {
			if (!(current->right)) {
				Left = 0; flag = 1;
			}
			else current = current->right;
		}
    }

    new = create_node ();
    if (!new) return (0);

    if (!Left) current->right = new;
    else current->left = new;

    P = createVector_char (len);
    if (!P) return (0);
    strcpy (P, pal);

    new->sT.word = P; new->sT.count = 1;
    new->parent = current; 
    new->left = NULL; new->right = NULL;
    ++*n;

    return (1);
}

void transferTo_vector (symTable *v, node *r, int *i)
{
   if (r != NULL) {
      transferTo_vector (v, r->left, i);
      v[*i] = r->sT; ++*i;
      transferTo_vector (v, r->right, i);
   }
}

void tabelaSimbolo_AB (char *fileName, int alph)
{
    FILE *fp;
    char *pal, c;
    node *root;
    int len = 0, maxLen = 50, n = 0, i = 0;
    symTable *v;
    
    fp = fopen (fileName, "r");
    if (!fp) {
        printf ("Nao foi possivel abrir o arquivo especificado.\n");
        return;
    } 
    pal = createVector_char (maxLen);
    root = create_node ();
    if (!pal || !root) goto error_noMem;

    root->parent = root->right = root->left = NULL;

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
            if (!insert_AB (root, &n, pal, len)) goto error_noMem;
            len = 0;     
        }
    }

    if (!alph) {
        v = createVector_symTable (n+2);
        transferTo_vector (v, root, &i);
        sort_symTable_O (v, 0, n - 1);
        sort_symtable_O_A (v, n);
        for (i = 0; i < n; i++) printf ("%s %u\n", v[i].word, v[i].count);
    }
    else print_lrr (root);
    destroy_tree (root);

    fclose (fp);
    destroyVector_char (pal);
    if (!n) destroy_node (root);
    else destroy_tree (root);
    return;

    error_noMem:
        printf ("Memoria insuficiente.\n");
        fclose (fp);
        destroyVector_char (pal);
        if (n) destroy_tree (root);
        else destroy_node (root);
        return;
}

