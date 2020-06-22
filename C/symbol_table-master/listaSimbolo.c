/* Autor: Sérgio Rosendo da Silva Júnior */
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "auxiliar_ep4.h"
#include "tabelaSimbolo_AB.h"
#include "tabelaSimbolo_LD.h"
#include "tabelaSimbolo_LO.h"
#include "tabelaSimbolo_VD.h"
#include "tabelaSimbolo_VO.h"

int main (int argc, char *arg[])
{
    int alph = 1, sorted = 1;
    if (argc != 4) {
        printf ("Numero insuficiente de argumentos da linha de comando.\n");
        return 0;
    }

    if (strcmp (arg[3], "O") == 0) alph = 0;
    if ((strcmp (arg[2], "VD") == 0) || (strcmp (arg[2], "LD") == 0)) 
        sorted = 0;

    if ((strcmp (arg[2], "VD") == 0) || (strcmp (arg[2], "VO") == 0)) 
        tabelaSimbolo_V (arg[1], sorted, alph);
    else if ((strcmp (arg[2], "LD") == 0) || (strcmp (arg[2], "LO") == 0))
        tabelaSimbolo_L (arg[1], sorted, alph);
    else tabelaSimbolo_AB (arg[1], alph);
    
    return 0;
}