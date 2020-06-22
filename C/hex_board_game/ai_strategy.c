/* Autor: Sérgio Rosendo da Silva Júnior */
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "auxiliar_ep5.h"
#include "board_management.h"
#include "ai_strategy.h"
#include "math.h"

/* matriz indicando o grau de prioridade do par linha coluna para a cor branca. Para a cor preta, usa-se a transposta 
desta matriz. */
int priorityBoard[14][14] = {
{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8}, 
{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7}, 
{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7},
{3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6},
{3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6},
{4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5},
{4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5},
{5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4},
{5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4},
{6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4, 3},
{6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4, 3},
{7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2},
{7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2},
{8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}
};


/* O peso de cada estrutura, detectada pela IA, na escolha da próxima jogada. Quanto mais alto o peso, mais a IA dará 
prioridade a preencher as células que, quando preenchidas, formam estas estruturas. */
int _cell_position_priority_multiplier = 4;
int _cell_is_dead_or_filled = 0;
int _cell_I_win = 900;
int _cell_enemy_win = 300;
int _cell_is_a_threatened_two_bridge = 15;
int _cell_can_generate_two_bridges = 7;
int _cell_creates_wall = 6;
int _cell_creates_rope = 3;

int isDead_cell_pattern_1 (char **board, int line, int column)
{
    int i, j;
    
    for (i = 0; i < 6; i++) {
        j = 0;
        while ((i <= 8) && isColor (board, line+neighbors[i%6].lin, column+neighbors[i%6].col, 'b')) {
            j++; i++;
            if (j == 4) return (1);
        }
    }

    for (i = 0; i < 6; i++) {
        j = 0;
        while ((i <= 8) && isColor (board, line+neighbors[i%6].lin, column+neighbors[i%6].col, 'p')) {
            j++; i++;
            if (j == 4) return (1);
        }
    }

    return (0);
}

int isDead_cell_pattern_2 (char **board, int line, int column)
{
    int i, j;
    
    for (i = 0; i < 6; i++) {
        j = 0;
        while ((i <= 8) && isColor (board, line+neighbors[i%6].lin, column+neighbors[i%6].col, 'b'))  {
            j++; i++;
            if ((j >= 3) && isColor (board, line+neighbors[(i+1)%6].lin, column+neighbors[(i+1)%6].col, 'p'))
                return (1);
        }
    }

    for (i = 0; i < 6; i++) {
        j = 0;
        while ((i <= 8) && isColor (board, line+neighbors[i%6].lin, column+neighbors[i%6].col, 'p')) {
            j++; i++;
            if ((j >= 3) && isColor (board, line+neighbors[(i+1)%6].lin, column+neighbors[(i+1)%6].col, 'b'))
                return (1);
        }
    }

    return (0);
}

int isDead_cell_pattern_3 (char **board, int line, int column)
{
    int i, j;
    
    for (i = 0; i < 6; i++) {
        j = 0;
        while ((i <= 8) && isColor (board, line+neighbors[i%6].lin, column+neighbors[i%6].col, 'b'))  {
            j++; i++;
            if ((j >= 2) && isColor (board, line+neighbors[(i+1)%6].lin, column+neighbors[(i+1)%6].col, 'p')
                && isColor (board, line+neighbors[(i+2)%6].lin, column+neighbors[(i+2)%6].col, 'p'))
                    return (1);
        }
    }

    for (i = 0; i < 6; i++) {
        j = 0;
        while ((i <= 8) && isColor (board, line+neighbors[i%6].lin, column+neighbors[i%6].col, 'p'))  {
            j++; i++;
            if ((j >= 2) && isColor (board, line+neighbors[(i+1)%6].lin, column+neighbors[(i+1)%6].col, 'b')
                && isColor (board, line+neighbors[(i+2)%6].lin, column+neighbors[(i+2)%6].col, 'b'))
                    return (1);
        }
    }

    return (0); 
}

int isDead_cell (char **board, int line, int column)
{
    if (isDead_cell_pattern_1 (board, line, column)) return (1);
    if (isDead_cell_pattern_2 (board, line, column)) return (2);
    if (isDead_cell_pattern_3 (board, line, column)) return (3);
    return (0);
}

void define_board_dead_cells (char **board)
{
    int line, column;

    for (line = 0; line < BOARD_LINES; line++)
        for (column = 0; column < BOARD_COLUMNS; column++)
            if (board[line][column] == '-')
                if (isDead_cell (board, line, column))
                    board[line][column] = 'D';

    return;
}

int **create_priorityBoard (char color)
{
    int **board, line, column;

    board = createMatrix_int (BOARD_LINES, BOARD_COLUMNS);
    if (!board) {
        destroyMatrix_int (board, 0);
        printf ("Memoria insuficiente\n"); 
        return (NULL);
    }

    if (color == 'b') 
        for (line = 0; line < BOARD_LINES; line++)
            for (column = 0; column < BOARD_COLUMNS; column++)
                board[line][column] = priorityBoard[line][column]*_cell_position_priority_multiplier;
    else 
        for (line = 0; line < BOARD_LINES; line++)
            for (column = 0; column < BOARD_COLUMNS; column++) /* matriz transposta de priorityBoard */
                board[line][column] = priorityBoard[column][line]*_cell_position_priority_multiplier;

    return board;
}

int cell_is_dead_or_filled (char **board, int line, int column)
{
    if (board[line][column] != '-') return (1);

    return (0);
}

int cell_win (char **board, int line, int column, char color)
{
    int i, j, return_value;
    char **temp_board;

    temp_board = createMatrix_char (BOARD_LINES, BOARD_COLUMNS);
    if (!temp_board) {
        destroyMatrix_char (temp_board, 0); 
        printf ("Memoria insuficiente\n"); 
        return (-1);
    }

    for (i = 0; i < BOARD_LINES; i++)
        for (j = 0; j < BOARD_LINES; j++)
            temp_board[i][j] = board[i][j];

    temp_board[line][column] = color;
    upper_connected_to_start (temp_board, line, column, color);
    
    if (check_victory (temp_board, color) == 1) return_value = (1);
    else return_value = (0);

    destroyMatrix_char (temp_board, BOARD_LINES);

    return return_value;
}

int cell_I_win (char **board, int line, int column, char my_color)
{
    return cell_win (board, line, column, my_color);
}

int cell_enemy_win (char **board, int line, int column, char enemy_color)
{
    return cell_win (board, line, column, enemy_color);
}

int cell_can_generate_two_bridges (char **board, int line, int column, char my_color)
{
    int i, count = 0;
    for (i = 0; i < 6; i++)
        if (isEmpty_and_live (board, line+neighbors[i].lin, column+neighbors[i].col))
            if (isEmpty_and_live (board, line+neighbors[(i+1)%6].lin, column+neighbors[(i+1)%6].col))
                if (isColor (board, line + neighbors[i].lin + neighbors[(i+1)%6].lin, column + neighbors[i].col + neighbors[(i+1)%6].col, my_color))
                    count++;

    return count;
}

int cell_is_a_threatened_two_bridge (char **board, int line, int column, char my_color, char enemy_color)
{
    int i, count = 0;
    for (i = 0; i < 6; i++)
        if (isColor (board, line + neighbors[i].lin, column + neighbors[i].col, my_color))
            if (isColor (board, line + neighbors[(i+1)%6].lin, column + neighbors[(i+1)%6].col, enemy_color))
                if (isColor (board, line + neighbors[(i+2)%6].lin, column + neighbors[(i+2)%6].col, my_color))
                    count++;

    return count;
}

int cell_creates_wall (char **board, int line, int column, char my_color)
{
    int i;
    if (my_color == 'b') {
        for (i = 0; i < 6; i++)
            if ((i != 2) && isColor (board, line + neighbors[i].lin, column + neighbors[i].col, my_color))
                return (1);
    }
    else {
        for (i = 1; i < 6; i++)
            if ((i != 3) && isColor (board, line + neighbors[i].lin, column + neighbors[i].col, my_color))
                return (1);
    }
    return (0);
}

int cell_creates_rope (char **board, int line, int column, char my_color)
{
    int i;
    if (my_color == 'b') {
        for (i = 1; i < 6; i++)
            if ((i != 3) && isColor (board, line + neighbors[i].lin, column + neighbors[i].col, my_color))
                return (1);
    }
    else {
        for (i = 0; i < 6; i++)
            if ((i != 2) && isColor (board, line + neighbors[i].lin, column + neighbors[i].col, my_color))
                return (1);
    }
    return (0);
}

int enhance_priority_board (char **board, int **priorityBoard, char my_color, char enemy_color)
{
    int line, column, temp, next = 0;

    for (line = 0; line < BOARD_LINES; line++)
        for (column = 0; column < BOARD_COLUMNS; column++, next = 0) {
            if (cell_is_dead_or_filled (board, line, column)) { /* celula irrelevante ou ja ocupada */
                priorityBoard[line][column] = _cell_is_dead_or_filled;
                next = 1;
            }
            if (!next) {
                temp = cell_I_win (board, line, column, my_color);
                if (temp == 1) { /* ocupar a celula acarreta em vitoria do jogador de cor my_color */
                    priorityBoard[line][column] += _cell_I_win;
                    next = 1;
                }
                else if (temp == -1) return (0); /* indica erro de falta de memoria */
            }
            if (!next) {
                temp = cell_enemy_win (board, line, column, enemy_color);
                if (temp == 1) { /* ocupar a celula com enemy_color acarreta em vitoria do jogador de cor enemy_color */
                    priorityBoard[line][column] += _cell_enemy_win = 300;
                    next = 1;
                }
                else if (temp == -1) return (0); /* indica erro de falta de memoria */
            }
            if (!next) {
                priorityBoard[line][column] = priorityBoard[line][column] + cell_is_a_threatened_two_bridge (board, line, column, my_color, enemy_color)*_cell_is_a_threatened_two_bridge;
                priorityBoard[line][column] = priorityBoard[line][column] + cell_can_generate_two_bridges (board, line, column, my_color)*_cell_can_generate_two_bridges;
                if (cell_creates_wall (board, line, column, my_color)) priorityBoard[line][column] += _cell_creates_wall;
                if (cell_creates_rope (board, line, column, my_color)) priorityBoard[line][column] += _cell_creates_rope;
            }
        }

    return (1);
}

index maxPriority (char **board, char my_color, char enemy_color)
{
    index pos;
    int line, column, max = 0, **my_priorityBoard, **enemy_priorityBoard;

    my_priorityBoard = create_priorityBoard (my_color);
    if (!my_priorityBoard) { /* retorna pos absurda para indicar erro de memoria*/
        destroyMatrix_int (my_priorityBoard, 0); pos.lin = -1; pos.col = -1; return pos; 
    }
    enemy_priorityBoard = create_priorityBoard (enemy_color);
    if (!enemy_priorityBoard) { /* retorna pos absurda para indicar erro de memoria*/
        destroyMatrix_int (my_priorityBoard, 0); pos.lin = -1; pos.col = -1; return pos;
    }

    if (!enhance_priority_board (board, my_priorityBoard, my_color, enemy_color)) goto error_noMem;

    /* Acha jogada com maior prioridade, baseando-se na soma das prioridades calculadas para cada jogados em cada posição */
    for (line = 0; line < BOARD_LINES; line++)
        for (column = 0; column < BOARD_COLUMNS; column++)
            if ((my_priorityBoard[line][column] + enemy_priorityBoard[line][column]) > max) {
                max = my_priorityBoard[line][column] + enemy_priorityBoard[line][column];
                pos.lin = line; pos.col = column;
            }

    if (max == 0 || !move_isValid (board, pos.lin, pos.col)) { /* caso a IA não tenha encontrado uma prioridade, gera uma jogada aleatoria, apenas para a prevenção
         de erro */
        line = rand() % BOARD_LINES; column = rand() % BOARD_COLUMNS;
        while (!move_isValid (board, line, column)) line = rand() % BOARD_LINES; column = rand() % BOARD_COLUMNS;
        pos.lin = line; pos.col = column;
    }

    destroyMatrix_int (my_priorityBoard, BOARD_LINES);
    destroyMatrix_int (enemy_priorityBoard, BOARD_LINES);
    return pos;

    error_noMem:
        destroyMatrix_int (my_priorityBoard, BOARD_LINES);
        destroyMatrix_int (enemy_priorityBoard, BOARD_LINES);
        pos.lin = -1; pos.col = -1; return pos;
}

int apply_pie_rule (int move_line, int move_column)
{
    if (priorityBoard[move_line][move_column] >= 6) return (1);
    return (0);
}

int make_my_move (char **board, char *my_color, char *enemy_color, int *moves_count)
{  
    index move_indexes;

    move_indexes = maxPriority (board, *my_color, *enemy_color);
    if ((move_indexes.lin == -1) && (move_indexes.lin == -1)) return (0);

    board[move_indexes.lin][move_indexes.col] = *my_color;
    upper_connected_to_start (board, move_indexes.lin, move_indexes.col, *my_color);
    printf ("%d %d\n", move_indexes.lin, move_indexes.col);
    ++*moves_count;
    return (1);
}

int receive_enemy_move (char **board, char *my_color, char *enemy_color, int *moves_count)
{
    int move_line, move_column, k;

    k = scanf ("%d %d", &move_line, &move_column);
    if (*moves_count == 1) { /* exceção para reconhecimento de possível pie rule aplicada pelo adversario*/
        while (!pos_isValid (move_line, move_column)) {
            printf ("Jogada invalida. Por favor, faça outra jogada.\n");
            k = scanf ("%d %d", &move_line, &move_column);
        }
        if (board[move_line][move_column] == *my_color || board[move_line][move_column] == toupper (*my_color)) { 
            /* adversario aplicou pie rule */
            swap_char (enemy_color, my_color);
            ++*moves_count;
            return (1);
        }
    }
    else
        while (!move_isValid (board, move_line, move_column)) {
            printf ("Jogada invalida. Por favor, faça outra jogada.\n");
            k = scanf ("%d %d", &move_line, &move_column);
        }
    
    board[move_line][move_column] = *enemy_color;
    upper_connected_to_start (board, move_line, move_column, *enemy_color);
    if (*moves_count == 0 && apply_pie_rule (move_line, move_column)) {
        swap_char (enemy_color, my_color); 
        ++*moves_count;
        return (0);    
    }    
    ++*moves_count; 
    if (k) return (1); /* apenas para prevenir scanf warning */
    return (1);
}