/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#ifndef _space_management_h
#define _space_management_h

#include "struct.h"
#include <stddef.h>
#include <stdio.h>

void inicializaQuickList();

void updateQuick();

FILE *new_memFile(char fileName[], ssize_t absSize);

Memory *new_Memory(char fileName[], ssize_t absSize);

Block *findHole(Memory *M, ssize_t size);

Block *newBlock(ssize_t absSize, Process *proc, Page *page, Memory *M);

void updateBlock(Block *b);

void mergeHoleBlocks(Block *b);

void releaseBlock(Block *b);

void freeBlock(Block *b);

void compressAll();

void compress(Memory *M);

void compressRec(Block *b);

void swapNextBlock(Block *b);

char readPos(Memory *M, ssize_t pos);

void printMemState(Memory *M);

void printAllMemState();

void freeMemory(Memory *M);

ssize_t absToPageCnt(ssize_t absSize);
//converts a absolut size into its required number of page frames

ssize_t absToAuCnt(ssize_t absSize);
//converts a absolut size into its required number of allocation units

ssize_t getPageFrameCnt(ssize_t absSize);

void freeR();

#endif
