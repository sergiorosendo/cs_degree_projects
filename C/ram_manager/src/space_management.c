/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#include "misc/array.h"
#include "misc/error.h"
#include "space_management.h"
#include "page_replacement.h"
#include <stdlib.h>
void updateQuick(){
	Block* tmp = vir->block;
	Block* tmp1 = quickList1;
	Block* tmp2 = quickList2;
	quickList1->nextQuick1 = NULL;
	quickList2->nextQuick2 = NULL;
	while (tmp !=NULL){
		if((tmp->size>=quickSize1) && (tmp->proc == NULL)){
			tmp1->nextQuick1 = tmp;
			tmp1=tmp;
			tmp-> nextQuick1 =NULL;
		}
		if((tmp->size>=quickSize2) && (tmp->proc == NULL)){
			tmp2->nextQuick2 = tmp;
			tmp2=tmp;
			tmp-> nextQuick2 =NULL;
		}
		tmp= tmp->next;
	}
}


void inicializaQuickList(){

	quickList1 = (Block*) emalloc(sizeof(Block)); 
	quickList2 = (Block*) emalloc(sizeof(Block));
	quickList1->nextQuick1 =NULL;
	quickList1->nextQuick2 = NULL;
	quickList2->nextQuick1 =NULL;
	quickList2->nextQuick2 = NULL;
	
	Block* tmp = vir->block;
	Block* tmp1 = quickList1;
	Block* tmp2 = quickList2;
	while (tmp !=NULL){
		if((tmp->size>=quickSize1) && (tmp->proc==NULL)){
			tmp1->nextQuick1 = tmp;
			tmp1=tmp;
			tmp-> nextQuick1 =NULL;
		}
		if((tmp->size>=quickSize2) && (tmp->proc==NULL)){
			tmp2->nextQuick2 = tmp;
			tmp2=tmp;
			tmp-> nextQuick2 =NULL;
		}
		tmp= tmp->next;
	}
}

Block *firstFit(Memory *M, ssize_t size) {
	Block *h;

    	for (h = M->block; h != NULL; h = h->next)
        	if (h->proc == NULL && h->size >= size)
	      		return h;
	return NULL;
}


Block *bestFit(Memory *M, ssize_t size) {
	Block *h;
	Block *best = NULL;
  
  // finds first hole
  for (h = M->block; h != NULL; h = h->next)
    if (h->proc == NULL && h->size >= size) {
      best = h;
      break;
    }

  // if first hole was found, try to find a smaller one
  if (best != NULL)
    for (h = h->next; h != NULL; h = h->next)
      if ((h->proc == NULL) && (h->size >= size) && (h->size < best->size))
        best = h;

	return best;
}

Block *worstFit(Memory *M, ssize_t size) {
	Block *h;
	Block *worst = NULL;
	ssize_t max = 0;
    	
	for (h = M->block; h != NULL; h = h->next){
        	if (h->proc == NULL && h->size >= size){
			if(h->size >= max){
				max = h->size;
				worst = h;
			}
		}
	}

	return worst;
}


Block *quickFit(Memory *M, ssize_t size) {
	Block *h;

	//printf("\tvai encaixar o processo de tamanho %lu\n",size);
	if (size == quickSize1){
		//printf("\t\tcoloco na lista quick 1\n");
		h = quickList1->nextQuick1;
	}
	else {
		if ((size == quickSize2) && (size != 0)){
			h = quickList2->nextQuick2;
	//		printf("\t\tcoloco na lista quick 2\n");
		}
		else{
			h = firstFit(M, size);
	//		printf("\t\tcoloco na lista else\n");
		}
	}
  
	return h;
}


FILE *new_memFile(char fileName[], ssize_t size) {
  FILE *mem = NULL; 
  char c = -1;

  mem = fopen(fileName, "w+b");
  if (!mem) {
    printErrNo();
    exit(EXIT_FAILURE);
  }

  for (ssize_t ii = 0; ii < size; ii++)
    fwrite(&c, sizeof(c), 1, mem);

  return mem;
}

Memory *new_Memory(char fileName[], ssize_t size) {
  Memory *M = NULL;
  ssize_t auCnt;

  M = (Memory *)emalloc(sizeof(Memory));
  M->mem = new_memFile(fileName, size);
  M->size = size;

  auCnt = absToAuCnt(size);

  M->bit_map = new_iArr(auCnt);
  set_iArr(0, M->bit_map, auCnt);

  //allocates the memory linked list's first block
  M->block = (Block *)emalloc(sizeof(Block));
  M->block->proc = NULL;
  M->block->pos = 0;
  M->block->size = auCnt;
  M->block->next = NULL;
  M->block->previous = NULL;
  M->block->page = NULL;
  M->block->M = M;
  M->block->absSize = M->size;

  return M;
}

Block *findHole(Memory *M, ssize_t size) {
  Block *h = NULL; int compressed = 0;

  while (h == NULL) {
    if (M == RAM) //finds first hole big enough for a single page frame
      h = firstFit(M, size);
    else 
      switch (freeSpaceAlg) {
        case (1): //best fit 
          h = bestFit(M, size); break;
        case (2): //worst fit
          h = worstFit(M, size); break;
        case (3): //quick fit
          h = quickFit(M, size); break;
        case (4): //extra algorithm
          h = firstFit(M, size); break;
        default:
          printError("undefined page swap algorithm");
          break;
      }

    //No hole big enough
    if (h == NULL) { 
      if (M == vir) {
        if (compressed == 1) {
          // virtual memory not big enough, even after compression
          printError("virtual memory not big enough");
          fprintf(stderr, "required space for block of %ld allocation units\n\n", size);
          printMemState(vir);
          exit(EXIT_FAILURE);
        }
        // try compressing virtual memory
        if (Time == 6) printMemState(vir);
        compress(M);
	if(freeSpaceAlg ==3)
		updateQuick();
        compressed = 1;
      }
      else {
        switch (pageSwapAlg) {
          // page swap algorithms
          case (1): //optimal
            removePage_optimal(); break;
          case (2): //fifo
           removePage_fifo(); break;
          case (3): //lru2
            removePage_lru2(); break;
          case (4): //lru4
            removePage_lru4(); break;
          case (5): //extra algorithm
            removePage_firstFound(); break;
          default:
            printError("undefined page swap algorithm");
            break;
        }
      }
    }
  }
  
  return h;
}

ssize_t absToPageCnt(ssize_t absSize) {
  ssize_t r;

  if (absSize%pageSize != 0)
    r = absSize/pageSize + 1;
  else
    r = absSize/pageSize;

  return r;    
}

ssize_t absToAuCnt(ssize_t absSize) {
  ssize_t r;

  if (absSize%auSize != 0)
    r = absSize/auSize + 1;
  else
    r = absSize/auSize;

  return r;  
}

Block *newBlock(ssize_t absSize, Process *proc, Page *page, Memory *M) {
  //inserts a new memory block inside current hole block
  Block *b = NULL, *current = NULL, *previous = NULL;
  ssize_t size;
  size = absToAuCnt(absSize);
  current = findHole(M, size);
  previous = current->previous;

  if (M == vir && current->size*auSize < absSize) {
    //printf("current->absSize = %ld, absSize = %ld;", current->size*auSize, absSize);
    //fflush(stdout);
    printError("process too big for virtual memory hole block\n");
    exit(EXIT_FAILURE);
  }
  if (page != NULL && current->size*auSize < pageSize) {
    printError("block to contain a single page is smaller than page size\n");
    exit(EXIT_FAILURE);
  }

  b = (Block *)emalloc(sizeof(Block));
  b->M = M;
  b->proc = proc;
  b->size = size;

  b->pos = current->pos;
  b->page = page;
  b->nextQuick1 =NULL;
  b->nextQuick2 = NULL;

  if (page != NULL && page->lastPage == 1 && proc->b%pageSize != 0)
    b->absSize = proc->b%pageSize;
  else
    b->absSize = absSize;

  b->absSize += b->absSize%auSize;

  b->previous = previous;
  if (previous != NULL)
    previous->next = b;

  if (page != NULL) {
    page->block = b; 
    if (M == RAM) 
      page->inRAM = 1;
  }
  current->size -= b->size;
  current->pos += b->size;

  if (current->size <= 0) {
    // b totally replaced current block
    b->next = current->next;
    if (b->next != NULL)
      b->next->previous = b;
    freeBlock(current);
  }
  else {
    b->next = current;
    current->previous = b;
  }

  if (b->pos == 0) {
    b->M->block = b;
    if (b->previous != NULL) {
      printError("head of linked list with previous node != NULL");
      exit(EXIT_FAILURE);
    }
  }

  if (M == vir) 
    proc->block = b;

  updateBlock(b);
  if (page != NULL)
    updateR(b);

  if(freeSpaceAlg ==3)
	  updateQuick();
  return b;
}

void updateBlock(Block *b) {
  char PID, c = -1;
  long int start, end1, end2, ii;
  Memory *M;

  M = b->M;
  if (b->proc == NULL)
    PID = -1;
  else
    PID = b->proc->PID;

  start = (long int)(b->pos*auSize);
  end1 = start + (long int)(b->absSize);
  end2 = start + (long int)(b->size*auSize);

  if (b->proc == NULL) {
    fseek(M->mem, start, SEEK_SET);
    for (ii = start; ii < end2; ii++)
      fwrite(&c, sizeof(c), 1, M->mem);
  }
  else {
    fseek(M->mem, start, SEEK_SET);
    for (ii = start; ii < end1; ii++)
      fwrite(&PID, sizeof(PID), 1, M->mem);
    for (ii = end1; ii < end2; ii++)
      fwrite(&c, sizeof(c), 1, M->mem);
  }

  if (b->proc == NULL)
    set_iArr(0, &(M->bit_map[b->pos]), b->size);
  else   
    set_iArr(1, &(M->bit_map[b->pos]), b->size);
}

void mergeHoleBlocks(Block *b) {
  Block *tmp;

  //merge with previous block
  if (b->previous != NULL) {
    if (b->previous->proc == NULL) {
      b->pos = b->previous->pos;
      b->size += b->previous->size;
      tmp = b->previous;
      b->previous = b->previous->previous;
      if (b->previous != NULL)
        b->previous->next = b;
      freeBlock(tmp);
    }
  }

  //merge with next block
  if (b->next != NULL) {
    if (b->next->proc == NULL) {
      b->size += b->next->size;
      tmp = b->next;
      b->next = b->next->next;
      if (b->next != NULL)
        b->next->previous = b;
      freeBlock(tmp);
    }
  }

  if (b->pos == 0) {
    b->M->block = b;    
    if (b->previous != NULL) {
      printError("head of linked list with previous node != NULL");
      exit(EXIT_FAILURE);
    }
  }
}

void releaseBlock(Block *b) {
  if (b == NULL || b->proc == NULL)
    return;

  if (b->page != NULL && b->M == RAM && b->page->inRAM == 1) {
    b->page->inRAM = 0;
    b->page->block = NULL;
    b->page = NULL;
    updateR(b);
  }
  
  if (b->M == vir) 
    b->proc->block = NULL;
  b->proc = NULL;

  updateBlock(b);
  mergeHoleBlocks(b);
}

void freeBlock(Block *b) {
  free(b);
  b = NULL;
}

void compressAll() {
  compress(RAM);
  compress(vir);
  if(freeSpaceAlg ==3)
  	updateQuick();
}

void compress(Memory *M) {
  Block *b;
  
  b = M->block;
  compressRec(b);
}

void compressRec(Block *b) {
  if (b == NULL)
    return;

  if (b->proc != NULL) //b contains a process
    return compressRec(b->next);

  if (b->proc == NULL) { //b is a hole
    mergeHoleBlocks(b);
    if (b->next == NULL) 
      return;
    swapNextBlock(b);
    return compressRec(b);
  }
}

void swapNextBlock(Block *b) {
  Block *a, *c, *d;

  c = b->next;
  if (c == NULL) {
    printError("trying to swap block with a non-existent next block");
    exit(EXIT_FAILURE);
  }

  swapR(b, c);

  a = b->previous;
  d = c->next;

  c->next = b;
  c->pos = b->pos;
  c->previous = a;

  b->next = d;
  b->pos = c->pos + c->size;
  b->previous = c;

  if (a) a->next = c;
  if (d) d->previous = b;

  updateBlock(c);
  updateBlock(b);

  if (c->pos == 0)
    c->M->block = c;

  //block list before: ...a->b->c->d...
  //new block list:    ...a->c->b->d...
}

char readPos(Memory *M, ssize_t pos) {
  char c;

  fseek(M->mem, pos, SEEK_SET);
  if (fread(&c, 1, 1, M->mem) == 1)
    return c;

  return (-2); //EOF
}

void printMemState(Memory *M) {
  ssize_t auCnt, rowCnt, row, coll, pos;
  char c;

  auCnt = absToAuCnt(M->size);

  //bitmap
  printf("\033[1m\033[37m" "%s" "\033[0m" " - Bitmap (t = %u):\n", M->name, Time);
  for (ssize_t ii = 0; ii < auCnt; ii++) {
    printf("%d", M->bit_map[ii]);
    if ((ii+1)%106 == 0)
      printf("\n");
  }
  printf("\n\n");

  //memory state
  printf("\033[1m\033[37m" "%s" "\033[0m" " - Estado (t = %u):\n", M->name, Time);
  rowCnt = M->size/4;
  for (row = 0; row < rowCnt; row++) {
    for (pos = row, coll = 0; coll < 5; pos += rowCnt, coll++)
      if (pos < M->size) {
        c = readPos(M, pos);
        if (M == RAM) {
          printf("\033[32m" "%ld:" "\033[0m" "\t%d\t\t", pos, c);
        }
        else {
          printf("\033[34m" "%ld:" "\033[0m" "\t%d\t\t", pos, c);
        }
      }
    printf("\n");
  }
  printf("\n");
  
  rewind(M->mem);
}

void printAllMemState() {
  printMemState(RAM);
  printMemState(vir);
}

void freeMemory(Memory *M) {
  Block *b, *c;

  for (b = M->block; b!= NULL; b = c) {
    c = b->next;
    freeBlock(b);
  }

  free_iArr(M->bit_map);
  fclose(M->mem);
  free(M);
  M = NULL;
}

ssize_t getPageFrameCnt(ssize_t absSize) {
  ssize_t r;

  if (absSize%pageSize != 0)
    r = absSize/pageSize + 1;
  else 
    r = absSize/pageSize;

  return r;
}

void freeR() {
  switch(pageSwapAlg) {
    case(1):
      free(filaFifo); 
      return;
    case(2):
      free(filaFifo); 
      return;
    case(3): //lru2
      free_iMat(lru2, pageFrameCnt);
      return;
    case(4): //lru4
      free(lru4);
      lru4 = NULL;
      return;
    default:
      return;
  }
}
