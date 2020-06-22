/* Autor: Sérgio Rosendo da Silva Júnior */
#ifndef _ai_strategy_h
#define _ai_strategy_h

extern int priorityBoard[14][14];

int isDead_cell_pattern_1 (char **board, int line, int column);
/* Retorna (1) se 4 vizinhos consecutivos (no sentido horario ou ant-horario) contiverem o mesmo caractere alfabetico 
('b' ou 'p'). */

int isDead_cell_pattern_2 (char **board, int line, int column);
/* Retorna (1) se 3 vizinhos consecutivos (no sentido horario ou ant-horario) contiverem o mesmo caractere alfabetico
 ('b' ou 'p') e o vizinho oposto a estes três contiver um caractere alfabetico diferente ('b' ou 'p'). */

int isDead_cell_pattern_3 (char **board, int line, int column);
/* Retorna (1) se 2 vizinhos consecutivos (no sentido horario ou ant-horario) contiverem o mesmo caractere alfabetico
 ('b' ou 'p') e os outros 2 vizinho consecutivos contiverem um caractere alfabetico diferente ('b' ou 'p'). */

int isDead_cell (char **board, int line, int column);
/* Retorna (1) se board[line][column] for considerada uma dead cell no tabuleiro de hex representado pela matriz board,
 retorna (0) caso contrário.*/

void define_board_dead_cells (char **board);
/* Altera, para 'D', o conteudo de toda cell da matriz board tabuleiro de hex que pode ser considerada como morta. */

int **create_priorityBoard (char color);
/* Devolve o ponteiro para uma matriz p[BOARD_LINES][BOARD_COLUMNS], os elementos de p indicam o grau de prioridade de
 uma posição relacionado a um tabuleiro de hex em relação a cor color (utilizado pela AI de hex.c).*/

int cell_is_dead_or_filled (char **board, int line, int column);
/* Retorna (1) se board[line][column], da matriz board representante de um tabuleiro de hex, é considerado uma celula
 morta ou se ja esta preenchido. */

int cell_win (char **board, int line, int column, char color);
/* Retorna (1) se board[line][column] == color acarreta em vitoria do jogador de cor color. Retorna (0) caso contrario 
ou retorna (-1) se houver ocorrido erro de memoria.*/

int cell_I_win (char **board, int line, int column, char my_color);
/* Retorna (1) se board[line][column] == my_color acarreta em vitoria do jogador de cor my_color. Retorna (0) caso 
contrario ou retorna (-1) se houver ocorrido erro de memoria. */

int cell_enemy_win (char **board, int line, int column, char enemy_color);
/* Retorna (1) se board[line][column] == enemy_color acarreta em vitoria do jogador de cor enemy_color. Retorna (0) 
caso contrario ou retorna (-1) se houver ocorrido erro de memoria. */

int cell_can_generate_two_bridges (char **board, int line, int column, char my_color);
/* Retorna o numero de 2-bridges formadas ao preencher board[line][column] com my_color. */

int cell_is_a_threatened_two_bridge (char **board, int line, int column, char my_color, char enemy_color);
/* Retorna o numero de 2-bridges em risco de serem completamente fechadas se board[line][column] não for preenchido com
 my_color. */

int cell_creates_wall (char **board, int line, int column, char my_color);
/* Retorna (1) se ao preencher board[line][column] com my_color, forma-se uma wall no sentido adequadoa my_color. 
Retorna (0) caso contrario. */

int cell_creates_rope (char **board, int line, int column, char my_color);
/* Retorna (1) se ao preencher board[line][column] com my_color, forma-se uma rope no sentido adequado a my_color. 
Retorna (0) caso contrario. */

int enhance_priority_board (char **hexBoard, int **priorityBoard, char my_color, char enemy_color);
/* Para cada posição vazia de hexBoard[][], aumenta ou diminui o valor de seu conteudo de acordo com seu grau de 
prioridade para o jogador com cor my_color. */

index maxPriority (char **board, char my_color, char enemy_color);
/* Devolve o index da posição mais vantajosa para o jogador de hex com cor my_color colocar sua peça ou devolve 
{-1, -1} se houve erro de memoria. */

int apply_pie_rule (int move_line, int move_column);
/* se a o conteudo da matriz global priorityBoard em [move_line][move_column] for >= 6 retorn (1). Se não retorna (0) */

int make_my_move (char **board, char *my_color, char *enemy_color, int *moves_count);
/* Se moves_count == 1, possivelmente troca o conteudo de *my_color com *enemy_color ("pie rule"). Altera uma 
posição de board tal que board[i][j] == '-' para board[i][j] = my_color. Retorna (1) se a matriz board foi alterada e 
(0) se houver erro de memoria. */

int receive_enemy_move (char **board, char *my_color, char *enemy_color, int *moves_count);
/* le da entrada padrão 2 inteiros 'i' e 'j', referentes a uma jogada de hex, se board[i][j] == '-' altera-o para 
enemy_color. Se board[i][j] == *my_color e moves_count == 1 troca o conteudo de *my_color com *enemy_color ("pie 
rule") e retorna (1), se não pede outra jogada. Se moves_count == 0 e tenha-se aplicado a pie rule retorna (0).*/

#endif