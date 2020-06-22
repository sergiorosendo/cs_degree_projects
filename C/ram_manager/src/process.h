/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#ifndef _process_h
#define _process_h

#include "struct.h"
#include <stddef.h>
#include <stdio.h>

Process init_Process(int PID);

void new_PosAcess(PosAcess **head, ssize_t pos, int t);

void delete_PosAcess(PosAcess *a);

int inRAM(Process *proc, ssize_t local_pos);

void acessPos(Process *proc);

void init_PageArr(Process *proc);

ssize_t getPageCnt(Process *proc);

ssize_t getPageId(ssize_t pos);

void finishProcess(Process *proc);

void freeProcesses();

void freeProcess(Process *p);

void tryFinishProcesses();

void startProcesses();

void startProcess(Process *proc);

#endif