/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#ifndef _page_replacement_h
#define _pace_replacement_h

#include "struct.h"
#include <stddef.h>

void updateR_fifo(Block* b);

void updateR_optimal(Block* b);

void updateR(Block *b);

void updateR_lru2(Block *b);

void swapR(Block *b, Block *c);

void swapR_lru2(Block *b, Block *c);

void resetR_lru2();

void removePage_lru4();

int ipow(int n, int p);

ssize_t binarySum(int *arr, ssize_t size);

void removePage_lru2();

void removePage_optimal();

void removePage_fifo();

ssize_t getPageFrameId(Block *b);

void updateR_lru4(Block *b);

void swapR_lru4(Block *b, Block *c);

void resetR_lru4();

void resetR();

void set_iColl(int x, ssize_t coll, int **mat, ssize_t rowCnt);

void collSwap(ssize_t coll1, ssize_t coll2, int **mat, ssize_t rowCnt);

void removePage_firstFound();

Block *firstFit(Memory *M, ssize_t size);

#endif
