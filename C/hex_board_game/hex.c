/* Autor: Sérgio Rosendo da Silva Júnior */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "auxiliar_ep5.h"
#include "board_management.h"
#include "ai_strategy.h"

void hexGame (int b, int show_board)
{
    char **board, my_color = 'b', enemy_color = 'p';
    int my_turn = 1, moves_count = 0, victory = 0;

    board = createMatrix_char (BOARD_LINES, BOARD_COLUMNS);
    if (!board) {
        destroyMatrix_char (board, 0);
        printf ("Memoria insuficiente\n"); 
        return;
    }
    define_board (board, '-');

    if (!b) {
        swap_char (&enemy_color, &my_color); my_turn = 0;
    }

    while (!victory)
    {
        if (my_turn) {
            define_board_dead_cells (board); 
            if (!make_my_move (board, &my_color, &enemy_color, &moves_count)) goto error_noMem;
            my_turn = 0;
        }
        else {
            if (show_board) print_board (board);
            if (!receive_enemy_move (board, &my_color, &enemy_color, &moves_count)) my_turn = 0;
            else my_turn = 1;
        }
        victory = check_victory (board, my_color);   
    }

    if (victory == 1) printf ("%c ganhou\n", my_color);
    else printf ("%c ganhou\n", enemy_color);

    destroyMatrix_char (board, BOARD_LINES);
    return;

    error_noMem:
        destroyMatrix_char (board, BOARD_LINES);
        return;
}

int main (int argc, char *arg[])
{
    int b = 1, show_board = 0;

    if (argc < 1) {
        printf ("Numero insuficiente de argumentos da linha de comando.\n");
        return 0;
    }

    if (strcmp (arg[1], "p") == 0) b = 0;
    if ((argc >= 2) && (strcmp (arg[2], "d") == 0)) show_board = 1;

    hexGame (b, show_board);
    return 0;
}