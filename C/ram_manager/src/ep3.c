/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#include "space_management.h"
#include "page_replacement.h"
#include "process.h"
#include "trace.h"
#include "ep3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//Global Variables
Memory *RAM;          //physical memory
Memory *vir;          //virtual memory
unsigned Time;        //pseudo Time passed in seconds
Process *P;           //process array
ssize_t Psize;        //process array size
unsigned *C;          //compress Time array
ssize_t Csize;        //compress array size
unsigned pfCnt;       //page faults counter
ssize_t auSize;       //allocation unit size
ssize_t pageSize;     //page size
ssize_t compressCnt;  //memory compressions done cnt
int freeSpaceAlg;     //free space manager algorithm id
int pageSwapAlg;      //page swap manager algorithm
unsigned interval;    //interval in seconds to print memories
int **lru2;           //lru2 matrix
unsigned char *lru4;  //lru4 R array
ssize_t pageFrameCnt; //RAM number of pageFrameCnt
Block** filaFifo;
int filaFifoCnt;
Hist* quickProcHist;
int histCnt;
ssize_t quickSize1;
ssize_t quickSize2;
Block* quickList1;
Block* quickList2;

int main(int argc, char **argv) 
{
  char traceFileName[1024];

  if (argc == 1) 
    ep3prompt();

  else {
    freeSpaceAlg = atoi(argv[1]);
    pageSwapAlg = atoi(argv[2]);
    strcpy(traceFileName, argv[3]);
    interval = (unsigned)atoi(argv[4]);
    ep3(traceFileName);
  }  

  exit(EXIT_SUCCESS);
}

void printMem(){
    Block *tmp = vir->block;
    printf("tempo %u\n",Time);
    printf("memoria virtual");
    while (tmp != NULL) {
	if (tmp->proc == NULL)
      		printf(" - %ld(%lu) (vazio)",tmp->size,tmp->pos);
	else	
      		printf(" - %ld(%lu)",tmp->size,tmp->pos);
       tmp = tmp->next;
    }
    printf("\n");
    if (freeSpaceAlg == 3) {
    	tmp = quickList1->nextQuick1;
    	printf("lista de tamanho %lu",quickSize1);
    	while (tmp != NULL) {
      		printf(" - %ld(%ld)",tmp->size,tmp->pos);
    	   	tmp = tmp->nextQuick1;
    	}
    	printf("\n");
    
    	tmp = quickList2->nextQuick2;
   	printf("lista de tamanho %lu",quickSize2);
    	while (tmp != NULL) {
      		printf(" - %ld(%ld)",tmp->size,tmp->pos);
       		tmp = tmp->nextQuick2;
    	}
    }
    printf("\n");
}

void ep3(const char traceFileName[]) {
  ssize_t ii;
  
  pfCnt = 0;
  compressCnt = 0;

  readTraceFile(traceFileName);
	
  if(freeSpaceAlg == 3){
	qsort(quickProcHist,histCnt,sizeof(Hist),ordena_hist);
	quickSize1 = quickProcHist[0].size;
	quickSize2 = quickProcHist[1].size;
        inicializaQuickList();
  }


  for (Time = 0;;) {
    startProcesses(); //start any new processes

    for (ii = 0; ii < Psize; ii++)
      //reference process pages, if any should be referenced
      acessPos(&P[ii]);
    
    while (compressCnt < Csize && C[compressCnt] <= Time) {
      //compress memories
      compressAll();
      compressCnt++;
    }

    resetR(); //downgrades every page frame R, if R is used

    if (interval != 0 && Time != 0 && Time%interval == 0)
      //print memories's current state
      printAllMemState();

    Time++;

    if (isDone())
      //finish any completed processes and check if all are done
      break;
  }

  if (interval != 0 && Time != 0 && Time%interval == 0)
    printAllMemState();

  printf("Pagefaults: %u\n",pfCnt);
  freeProcesses();
  freeMemory(vir);
  freeMemory(RAM);
  freeR();
  free(C);
  return;
}

void ep3prompt() {
  char cmd[1024];
  char traceFileName[1024];
  char *fsaName[4] = {"Best Fit", "Worst Fit", "Quick Fit", "First Fit"};
  char *psaName[5] = {"Optimal", "First-In, First-Out", "Least Recently Used 2", "Least Recently Used 4", "First Found"};
  int len;

  srand(time(NULL));
  freeSpaceAlg = 4;
  pageSwapAlg = 5;
  interval = 1; 

  while (1) {
    fprintf(stderr, "[ep3]: ");
    if (!fgets(cmd, 1024, stdin)) 
      continue;
    if (strncmp("\n", cmd, 1) == 0) //empty command
      continue;
    if (strncmp("carrega", cmd, 7) == 0) {
      strcpy(traceFileName, &cmd[8]);
      len = strlen(traceFileName);
      if (len > 0 && traceFileName[len-1] == '\n') {
        traceFileName[len-1] = '\0';
      
      fprintf(stderr, ">trace \"%s\" carregado\n", traceFileName); }
      continue;
    }
    if (strncmp("espaco", cmd, 6) == 0) {
      freeSpaceAlg = atoi(&cmd[7])%5;
      fprintf(stderr, ">\"%s\" (gerenciamento de espaco livre)\n", fsaName[(freeSpaceAlg-1)%4]);
      continue;
    }
    if (strncmp("substitui", cmd, 9) == 0) {
      pageSwapAlg = atoi(&cmd[10])%6;
      fprintf(stderr, ">\"%s\" (substituicao de pagina)\n", psaName[(pageSwapAlg-1)%5]);
      continue;
    }
    if (strncmp("executa", cmd, 7) == 0) {
      interval = atoi(&cmd[8]);
      ep3(traceFileName);
      continue;
    }
    if (strncmp("random", cmd, 6) == 0) {
      interval = atoi(&cmd[7]);
      for (int xx = 1; xx <= 20; xx++) {
        random_trace();
        int yy = 1;
        for(pageSwapAlg = 2; pageSwapAlg < 5; pageSwapAlg++)
          for(freeSpaceAlg = 1; freeSpaceAlg < 5; freeSpaceAlg++) {
            fprintf(stderr, ">Random Trace %d - Test %d of 12: \"%s\" + \"%s\"\n", xx, yy, fsaName[freeSpaceAlg-1], psaName[pageSwapAlg-1]);      
            ep3("trace");
            yy++;
          }
      }
      freeSpaceAlg = 4;
      pageSwapAlg = 4;
      continue;
    }
    if (strncmp("sai", cmd, 3) == 0)
      return;
    fprintf(stderr, ">comando invalido\n");
  }
}

int isDone() {
  tryFinishProcesses();

  if(freeSpaceAlg == 3)
	  updateQuick();

  if (compressCnt < Csize)
    return 0;

  for (ssize_t ii = 0; ii < Psize; ii++)
    if (P[ii].isFinished == 0)
      return 0;

  return 1;
}

