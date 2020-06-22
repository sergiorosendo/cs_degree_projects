/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#include "misc/array.h"
#include "misc/error.h"
#include "page_replacement.h"
#include "space_management.h"
#include <stdlib.h>
#include <stdio.h>

void updateR(Block *b) {
  if (b == NULL) return;
  if (b->M == vir) return;

  switch (pageSwapAlg) {
    case (1): //optimal
      updateR_optimal(b);
      break;
    case (2): //fifo
      updateR_fifo(b);
      break;
    case (3): //lru2
      updateR_lru2(b);
      break;
    case (4): //lru4
      updateR_lru4(b);
      break;
    case (5): //first found
      return;
  }
}

void removeFromFifo(Block *b){
  int i;
  
  if (b->page != NULL) {
    printError("trying to block with no page from queue");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < filaFifoCnt; i++)
    if (filaFifo[i] == b)
      break;

  if (i == filaFifoCnt) {
    printError("trying to remove unexistent block from queue");
    exit(EXIT_FAILURE);
  }

  for(int j = i; j < filaFifoCnt - 1; j++)
    filaFifo[j] = filaFifo[j+1];
  
  filaFifo[filaFifoCnt-1] = NULL;
  filaFifoCnt -= 1;
}

int findNext (PosAcess *h){ 
  int t = -1;
  PosAcess *temp = h->next;

  ssize_t p = (h->local_pos)/pageSize;

  while(temp != NULL){
    ssize_t np = (temp->local_pos)/pageSize;
    if (np == p) {
      t = (int)temp->t;
      break;
    }
    temp = temp->next;
  }

  return t;

}

int compara(const void* a, const void* b){

  Block* x = *((Block**) a);
  Block* y = *((Block**) b);
  
  if(x->next_access == -1)
    return -1;
  else{
    if(y->next_access == -1)
      return 1;
    else
      return y->next_access -  x->next_access; 
  }
}


void putOrder(Block *b){
  int i;

  for (i=0; i < filaFifoCnt;i++) 
    if (filaFifo[i]==b)
      break;

  if (i < filaFifoCnt) {
    for (int j = i; j < (filaFifoCnt -1);j++)
      filaFifo[j] = filaFifo[j+1];
    filaFifo[filaFifoCnt-1] = b;
    b->next_access = findNext(b->proc->A);
    return;
  }

  else {  
    if (filaFifoCnt >= pageFrameCnt) {
      printError("trying to insert new element into queue with no free space");
      exit(EXIT_FAILURE);
    }
    filaFifo[filaFifoCnt] = b;
    filaFifoCnt++;
  }
  b->next_access = findNext(b->proc->A);
  qsort(filaFifo,filaFifoCnt,sizeof(Block*),compara);

}
void updateR_optimal(Block *b) {
  if (b == NULL || b->M == vir) {
    printError("trying to update R from a NULL or a virtual memory block\n");
    exit(EXIT_FAILURE);
  }

  if (b->page == NULL) //page frame was just freed
    removeFromFifo(b); //remove b da fila

  if (b->page != NULL)
    putOrder(b); //se b esta na fila, remove e poe no final
    //se nao esta, só poe no final

  return;

}



void putLast(Block *b){
  int i;

  for (i=0; i < filaFifoCnt;i++) 
    if (filaFifo[i]==b)
      break;

  if (i < filaFifoCnt) {
    for (int j = i; j < (filaFifoCnt -1);j++)
      filaFifo[j] = filaFifo[j+1];
    filaFifo[filaFifoCnt-1] = b;
    return;
  }

  else {  
    if (filaFifoCnt >= pageFrameCnt) {
      printError("trying to insert new element into queue with no free space");
      exit(EXIT_FAILURE);
    }
    filaFifo[filaFifoCnt] = b;
    filaFifoCnt++;
  }
  
}

void updateR_fifo(Block *b) {
  if (b == NULL || b->M == vir) {
    printError("trying to update R from a NULL or a virtual memory block\n");
    exit(EXIT_FAILURE);
  }

  if (b->page == NULL) //page frame was just freed
    removeFromFifo(b); //remove b da fila

  if (b->page != NULL)
    putLast(b); //se b esta na fila, remove e poe no final
    //se nao esta, só poe no final

  return;
}


void updateR_lru2(Block *b) {
  size_t row, coll;
  if (pageSwapAlg != 3) return;

  row = getPageFrameId(b);
  coll = row;
  
  if (b->page == NULL) 
    //page frame just became empty 
    set_iArr(0, lru2[row], pageFrameCnt);
  else{ 
    //page frame was just referenced
    set_iArr(1, lru2[row], pageFrameCnt);
    set_iColl(0, coll, lru2, pageFrameCnt);
  }
}

void swapR(Block *b, Block *c) {
  if (b != NULL && b->M != RAM) return;

  switch (pageSwapAlg) {
    /*case(1):
      break;
    case(2):
      break;*/
    case(3):
      swapR_lru2(b, c);
      break;
    case(4):
      swapR_lru4(b, c);
      break;
    default:
      break;
  }
}

void swapR_lru2(Block *b, Block *c) {
  ssize_t b_id, c_id;
  int *tmp;

  if (b->M != RAM) return;
  if (pageSwapAlg != 3) return;

  b_id = getPageFrameId(b);
  c_id = getPageFrameId(c);

  //swap lines
  tmp = lru2[b_id];
  lru2[b_id] = lru2[c_id];
  lru2[c_id] = tmp;

  //swap collumns
  collSwap(b_id, c_id, lru2, pageFrameCnt);
}

void resetR_lru2() {
  set_iMat(0, lru2, pageFrameCnt, pageFrameCnt);
}
void removePage_optimal(){
  removePage_fifo();
}

void removePage_fifo(){
  Block *b;

  if (filaFifoCnt == 0) {
    printError("trying to remove block from an empty queue");
    exit(EXIT_FAILURE);
  }

  b = filaFifo[0];

  releaseBlock(b);
}

void removePage_lru4() {
  Block *h, *b;
  ssize_t h_id, b_id;

  b = RAM->block;
  while (b->page == NULL) {
    b = b->next;
    if (b == NULL) {
      printError("trying to find page frame in empty RAM");
      exit(EXIT_FAILURE);
    }
  }

  for (h = b, b = b->next; b != NULL; b = b->next) {
    if (b->page != NULL) {
      h_id = getPageFrameId(h);
      b_id = getPageFrameId(b);
      if (lru4[b_id] < lru4[h_id])
        h = b;
    }
  }

  releaseBlock(h);
}

int ipow(int n, int p) {
  int m;

  if (p == 0)
    return 1;

  m = n;
  while (p-- > 1)
    n *= m;

  return n;
}

ssize_t binarySum(int *arr, ssize_t size) {
  int sum = 0, ii, jj;
  
  for (ii = 0, jj = (int)size - 1; ii < (int)size; ii++, jj--)
    sum += ipow(2, jj)*arr[ii];

  return sum;
}

void removePage_lru2() {
  Block *h, *b;
  ssize_t h_id, b_id;

  b = RAM->block;
  while (b->page == NULL) {
    b = b->next;
    if (b == NULL) {
      printError("trying to find page frame in empty RAM");
      exit(EXIT_FAILURE);
    }
  }

  for (h = b, b = b->next; b != NULL; b = b->next)
    if (b->page != NULL) {
      h_id = getPageFrameId(h);
      b_id = getPageFrameId(b);
      if (binarySum(lru2[b_id], pageFrameCnt) < binarySum(lru2[h_id], pageFrameCnt))
        h = b;
    }
  
  releaseBlock(h);
}

ssize_t getPageFrameId(Block *b) {
  if (b == NULL) {
    printError("trying to acess empty block's page frame");
    exit(EXIT_FAILURE);
  }

  if (b->M == vir) {
    printError("tring to acesss page frame from virtual memory block");
    exit(EXIT_FAILURE);
  }

  ssize_t id;

  id = (b->pos*auSize)/pageSize;

  if (id >= pageFrameCnt) {
    printError("page frame id is greater than maximum");
    exit(EXIT_FAILURE);
  }

  return id;
}

void updateR_lru4(Block *b) {
  if (b == NULL) return;
  if (b->M == vir) return;
  if (pageSwapAlg != 4) return;

  ssize_t id;
  id = getPageFrameId(b);

  if (b->page == NULL)
    //page frame became empty
    lru4[id] = 0;
  else {
    //page frame was referenced
    if (lru4[id] >= 128) 
      //page was referenced more than once in current time step
      lru4[id] >>= 1;
    lru4[id] += 128;     
  }
}

void swapR_lru4(Block *b, Block *c) {
  ssize_t b_id, c_id;
  unsigned char tmp;

  b_id = getPageFrameId(b);
  c_id = getPageFrameId(c);

  tmp = lru4[b_id];
  lru4[b_id] = lru4[c_id];
  lru4[c_id] = tmp;
}

void resetR_lru4() {
  for (ssize_t ii = 0; ii < pageFrameCnt; ii++)
    lru4[ii] >>= 1;
}

void resetR() {
  switch (pageSwapAlg) {
    /*case(1): //optimal
      return; 
    case(2): //fifo
      return;
    case(3): //lru2 
      return; */
    case(4): //lru4
      resetR_lru4();
      return;
    default: //first found
      return;
  }
}

void set_iColl(int x, ssize_t coll, int **mat, ssize_t rowCnt) {
  ssize_t row;

  for (row = 0; row < rowCnt; row++)
    mat[row][coll] = x;
}

void collSwap(ssize_t coll1, ssize_t coll2, int **mat, ssize_t rowCnt) {
  ssize_t row;
  int tmp;

  for (row = 0; row < rowCnt; row++) {
    tmp = mat[row][coll1];
    mat[row][coll1] = mat[row][coll2];
    mat[row][coll2] = tmp;
  }
}

void removePage_firstFound() {
  Block *b;

  for (b = RAM->block; b != NULL; b = b->next)
    if (b->proc != NULL) {
      releaseBlock(b);
      return;
    }
  
  printError("could not release any block, no filled blocks were found");
  exit(EXIT_FAILURE);
}


