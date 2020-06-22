/* Autor: Sérgio Rosendo da Silva Júnior */
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "auxiliar_ep5.h"
#include "board_management.h"

index neighbors[6] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 0}, {1, -1}, {0, -1}};
/* indicam a pos relativa {lin, col} de cada vizinho de uma posição b[x][y] de um tabuleiro de hex representado por uma
 matriz b. */

void print_board (char **board)
{
    int line, column, t;
    for (line = 0; line < BOARD_LINES; line++) {
        for (t = 0; t < line; t++) 
            fprintf(stderr, "   ");
        for (column = 0; column < BOARD_COLUMNS; column++) 
            if (board[line][column] == 'D') fprintf(stderr, "-     "); 
            /* A celula morta ('D' de dead) é usada apenas para melhor performance da IA, trata-se de uma celula vazia e
             deve ser impressa como tal. */
            else fprintf(stderr, "%c     ", tolower (board[line][column]));
        fprintf(stderr, "\n\n");
    }

    fprintf(stderr, "\n");
    return;
}

void define_board (char **board, char c)
{
    int line, column;

    for (line = 0; line < BOARD_LINES; line++)
        for (column = 0; column < BOARD_COLUMNS; column++)
            board[line][column] = c;
    
    return;
}

int check_victory (char **board, char my_color)
{
    int i;

    for (i = 0; i < BOARD_COLUMNS; i++)
        if (board[BOARD_LINES-1][i] == 'B') {
            if (my_color == 'b') /* a IA ganhou */
                return (1);
            else /* adversario ganhou */
                return (-1);
        }

    for (i = 0; i < BOARD_LINES; i++)
        if (board[i][BOARD_COLUMNS-1] == 'P') {
            if (my_color == 'p') /* a IA ganhou */
                return (1);
            else /* adversario ganhou */
                return (-1);
        }

    return (0); /* jogo ainda não acabou */
}

int pos_isValid (int line, int column)
{
    if (inInterval (line, 0, BOARD_LINES - 1) && inInterval (column, 0, BOARD_COLUMNS - 1))
        return (1);
    else
        return (0);
}

int pos_isValid_withBorders (int line, int column)
{
    if (inInterval (line, -1, BOARD_LINES) && inInterval (column, -1, BOARD_COLUMNS))
        return (1);
    else
        return (0);
}

int move_isValid (char **board, int move_line, int move_column)
{
    char c;
    if (!pos_isValid (move_line, move_column)) return (0);

    c = tolower (board[move_line][move_column]);
    if (c == 'b' || c == 'p') return (0);

    return (1);
}

int check_neighbors (char **board, int line, int column, char c)
{
    int i;
    for (i = 0; i < 6; i++)
        if (pos_isValid (line + neighbors[i].lin, column + neighbors[i].col))
            if (board[line+neighbors[i].lin][column+neighbors[i].col] == c)
                return (1);

    return (0);
}

void upper_connected_to_start (char **board, int line, int column, char c)
{
    int i;
    if (!pos_isValid (line, column)) return;
    if (board[line][column] != c) return; /* posição livre, ocupada por char referente a peça do adversario ou já 
    marcada como conectada ao início (ou seja, contém 'B' ou 'P') */
        

    if (((line == 0) && (c == 'b')) || ((column == 0) && (c == 'p')) 
        || check_neighbors (board, line, column, toupper (c))) {
        board[line][column] = toupper (board[line][column]);
        for (i = 0; i < 6; i++) 
            /* chama upper_connected_to_start para cada vizinho de board[line][column] */
            upper_connected_to_start (board, line + neighbors[i].lin, column + neighbors[i].col, c); 
    }

    return;
}

int isColor (char **board, int line, int column, char color)
{
    if (pos_isValid (line, column) && (tolower (board[line][column]) == color))
        return (1);
    else if (pos_isValid_withBorders (line, column)) {
        if ((color == 'b') && ((line == -1) || (line == BOARD_LINES)))
            return (1); /* "borda branca" do tabuleiro */
        else if ((color == 'p') && ((column == -1) || (column == BOARD_COLUMNS)))
            return (1); /* "borda preta" do tabuleiro */
    }

    return (0);
}

int isEmpty_and_live (char **board, int line, int column)
{
    if (pos_isValid (line, column) &&  (board[line][column] == '-'))
        return (1);
    return (0);
}
