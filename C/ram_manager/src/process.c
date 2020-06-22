/* Guilherme Feulo do Espirito Santo - 6431749
	quickList1->nextQuick1 = NULL;
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#include "misc/array.h"
#include "misc/error.h"
#include "process.h"
#include "space_management.h"
#include "page_replacement.h"
#include <stdio.h>
#include <stdlib.h>
#include "ep3.h"
void acessPos(Process *proc) {
  PosAcess *a, *next;
  Page *page;
  if (proc->block == NULL) //process not in virtual memory (i.e. not running)
    return;
  
  for (a = proc->A; a != NULL && a->t <= Time; a = next) {
    page = &(proc->page[getPageId(a->local_pos)]);
    if (a->local_pos > proc->b) {
      printError("process trying to acess unexistent local_pos");
      exit(EXIT_FAILURE);
    }
    if (page->inRAM == 0) {
      //page fault
      pfCnt++;
      newBlock(pageSize, proc, page, RAM);
    }
    else {
      updateR(page->block);
    }
    next = a->next;
    delete_PosAcess(a);
  }

  proc->A = a;
}

Process init_Process(int PID) {
  Process proc;
  
  proc.PID = (char)PID;
  proc.A = NULL;
  proc.isFinished = 0;
  proc.block = NULL;
  proc.t0 = 0;
  proc.tf = 0;
  proc.b = -1;
  proc.A = NULL;
  proc.page = NULL;

  return proc;
}

void new_PosAcess(PosAcess **head, ssize_t pos, int t) {
  PosAcess *a = NULL, *b = NULL;

  a = (PosAcess *)emalloc(sizeof(PosAcess));
  a->local_pos = pos;
  a->t = t;
  a->next = NULL;

  if (*head != NULL) {
    for (b = *head; b->next != NULL; b = b->next);
    b->next = a;
  }
  else
    *head = a;
}

void delete_PosAcess(PosAcess *a) {
  if (a != NULL) {
    a->local_pos = 0;
    a->t = 0;
  }
  free(a);
  a = NULL;
}

int inRAM(Process *proc, ssize_t local_pos) {
  ssize_t id;

  id = getPageId(local_pos);
  return proc->page[id].inRAM;
}

void init_PageArr(Process *proc) {
  Page *page;
  ssize_t pageCnt;

  pageCnt = getPageCnt(proc);

  page = (Page *)emalloc(((size_t)pageCnt)*sizeof(page_struct));

  for (ssize_t ii = 0; ii < pageCnt; ii++) {
    page[ii].id = ii;
    page[ii].lastPage = 0;
    page[ii].proc = proc;
    page[ii].inRAM = 0;
    page[ii].block = NULL;
  }
  page[pageCnt-1].lastPage = 1;

  proc->page = page;
}

ssize_t getPageCnt(Process *proc) {
  ssize_t pageCnt;

  if (proc->b%pageSize != 0)
    pageCnt = proc->b/pageSize + 1;
  else
    pageCnt = proc->b/pageSize;

  return pageCnt;
}

ssize_t getPageId(ssize_t pos) {
  return pos/pageSize;
}

void finishProcess(Process *proc) {
  if (Time < proc->tf) {
    printError("trying to finish process before its tf");
    exit(EXIT_FAILURE);
  }

  ssize_t pageCnt;
  pageCnt = getPageCnt(proc);

  for (ssize_t ii = 0; ii < pageCnt; ii++)
    if (proc->page[ii].inRAM == 1) {
      if (proc->page[ii].block == NULL || proc->page[ii].block->page == NULL) {
        printError("page in RAM with NULL block or ram block with NULL page");
        exit(EXIT_FAILURE);
      }
      releaseBlock(proc->page[ii].block); //page's block in physical memory
    }
  
  releaseBlock(proc->block); //process's block in the virtual memory
  proc->isFinished = 1;
}

void freeProcesses() {
  for (ssize_t ii = 0; ii < Psize; ii++)
    freeProcess(&P[ii]);
  free(P);
  P = NULL;
}

void freeProcess(Process *p) {
  free(p->page);
  p->page = NULL;
}

void tryFinishProcesses() {
  ssize_t ii;

  for (ii = 0; ii < Psize; ii++)
    if (Time >= P[ii].tf)
      finishProcess(&P[ii]);
}

void startProcesses() {
  for (ssize_t ii = 0; ii < Psize; ii++)
    startProcess(&P[ii]);
}

void startProcess(Process *proc) {
  if (proc->block != NULL) return; //already in virtual memory
  if (Time < proc->t0) return;     //not time to start yet
  if (Time >= proc->tf) return;    //process finished already
  //ssize_t var;
  //if(proc->b%auSize==0)
  //	var = proc->b/auSize;
  //else
 //	var = proc->b/auSize+1;
  //printf("vai colocar  o proc %d de tamanho %lu (%lu)unidade de alocação)\n", (int) proc->PID,proc->b,var);
//printf("memoria antes\n");  
//printMem();
  proc->block = newBlock(proc->b, proc, NULL, vir);
//printf("memoria depois\n");  
//  printMem();
}
