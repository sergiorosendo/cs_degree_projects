/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _board_management_h
#define _board_management_h

#define BOARD_LINES 14
#define BOARD_COLUMNS 14
/* Tamanho do tabuleiro de hex a ser criado */

typedef struct {
    int lin;
    int col;
} index;

extern index neighbors[6];

void print_board (char **m);
/* Imprime os elementos da matriz m[0...(line-1)][0...(coll-1)]. */

void define_board (char **m, char c);
/* Define todos elemetos da matriz m[0...(line-1)][0...(coll-1)] como 'c'. */

int check_victory (char **board, char my_color);
/* Retorna (1) se algum elemento da linha board[BOARD_LINES-1][0...(BOARD_COLUMNS-1)]
 for igual a 'B' e my_color == 'b' ou (-1) se my_color == 'p'. Também retorna (1) se algum elemento
 da coluna board[0...(BOARD_LINES-1)][(BOARD_COLUMNS-1)] for igual a 'P' e my_color == 'p' ou (-1)
 se my_color == 'b'. Retorna (0) se nenhuma das condições anteriores forem atendidas. */

int pos_isValid (int line, int column);
/* Retorna (1) se line e coll são menores que BOARD_LINES e BOARD_COLUMNS resp
ectivamente, retorna (0) caso contrario. */

int move_isValid (char **board, int move_line, int move_column);
/* Retorna (1) se (0 <= move_line < BOARD_LINES), (0 <= move_column < BOARD_COLUMNS) e 
board[move_line][move_column] == '-', caso alguma dessas condições não seja satisf
eita, retorna (0). */

int check_neighbors (char **board, int line, int column, char c);
/* retorna (1) se, na matriz tabuleiro de hex 'board', pelo menos um dos seis 
vizinhos de board[line][column] tiver conteudo igual a 'c'. Retorna (0) caso contrario. */

void upper_connected_to_start (char **board, int line, int column, char c);
/* se board[line][column] == 'b' e esta posição estiver conectada a linha 
board[0][~] através de uma corrente contendo 'b' ou 'B', altera o conteudo de 
toda posição nessa corrente para 'B'. Analogamente, se board[line][column] == 'p' e esta p
osição estiver conectada a coluna board[~][0] através de uma corrente contendo
'p' ou 'P', altera o conteudo de posição nesta corrente para 'P'. */

int isColor (char **board, int line, int column, char color);
/* Retorna (1) se board[line][column] == color, retorna (0) caso contrário. 
Para efeitos de comparação, considera a linha (-1) e a linha (BOARD_LINES) como contendo 'b'
e considera a coluna (-1) e a coluna (BOARD_COLUMNS) como contendo 'p'.*/

int isEmpty_and_live (char **board, int line, int column);
/* Retorna (1) se uma celula do tabuleiro hex Board está vazia e não é considerada como celula 
morta (ou seja, não é uma celula irrelevante para o resultado do jogo). Retorna (0) caso contrario.*/

#endif