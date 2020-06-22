/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#include "trace.h"
#include "space_management.h"
#include "misc/error.h"
#include "misc/array.h"
#include "space_management.h"
#include "page_replacement.h"
#include "process.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

int ordena_hist(const void* a, const void* b){
	Hist* x = (Hist*) a;
	Hist* y = (Hist*) b;

        return y->cnt - x->cnt;


}
void readPosAcess(char *token, Process *proc) {
  ssize_t pos; int t, cnt = 0;

  for (token = strtok(NULL, " \n"); token != NULL; token = strtok(NULL, " \n"), cnt = (cnt + 1)%2) {
    if (cnt == 0)
      sscanf(token, "%ld", &pos);
    else {
      sscanf(token, "%d", &t);
      new_PosAcess(&proc->A, pos, t);
    }
  }
}

void putHist (ssize_t b){
	int i;

	int s_size;
	if(b%auSize==0)
		s_size= b/auSize;
	else
		s_size = (b/auSize)+1;
	for(i=0;i<histCnt;i++)
		if(quickProcHist[i].size == s_size)
			break;
	if(i==histCnt){
		quickProcHist[i].size = s_size;
		quickProcHist[i].cnt = 1;
		histCnt++;
	}
	else
		(quickProcHist[i].cnt)++;
}

void readProcess(char *line, Process *proc, int PID) {
  char *token; int cnt;

  *proc = init_Process(PID);

  token = strtok(line, " \n");
  
  for (cnt = 0; token != NULL; cnt++, token = strtok(NULL, " \n")) {
    switch (cnt) {
      case 0: 
        sscanf(token, "%u", &(proc->t0));
        break;
      case 1:
        sscanf(token, "%u", &(proc->tf));
        break;
      case 2:
        sscanf(token, "%ld", &(proc->b));
	if(freeSpaceAlg == 3)
		putHist(proc->b);
        init_PageArr(proc);
	
        break;
      case 3:
        sscanf(token, "%s", proc->name);
        readPosAcess(token, proc);
        return;
      default:
        printError("failed to read process from trace");
        exit(EXIT_FAILURE);
    }
  }
}

void readTraceFileProcesses(FILE *traceFile) {
  char *line = NULL; size_t n = 16192;
  int ii = 0, jj = 0;
  while (1) {
    if (getline(&line, &n, traceFile) == -1 || line[0] == '\n') {
      free(line);
      line = NULL;
      return;
    }
    if (strstr(line, "COMPACTAR") != NULL)
      sscanf(line, "%u", &C[jj++]); 
    else {
      readProcess(line, &P[ii], ii);
      ii++;
    }
  }
}

void scanTraceFile(FILE *traceFile) {
  Psize = 0; Csize = 0;
  char line[1024];
  char *ret = NULL;
 
  rewind(traceFile);
  
  while (fgets(line, 1024, traceFile) != NULL) {
    if (line[0] == '\n') break;
    ret = strstr(line, "COMPACTAR");
    if (ret == NULL)
      Psize += 1;
    else
      Csize += 1;
  }

  Psize -= 1; //ignores traceFile's first line

  rewind(traceFile);
}

void readTraceFile(const char traceFileName[]) {
  FILE *traceFile = NULL;
  char line[2048];
  ssize_t ramSize, virSize, ii;

  if (freeSpaceAlg==3){
    quickProcHist = (Hist*) malloc(127*sizeof(Hist));
    histCnt =0;
	for(int i=0;i<127;i++){
		quickProcHist[i].size = 0;
		quickProcHist[i].cnt = 0;
	}
  }
  traceFile = fopen(traceFileName, "r");
  if (!traceFile) {
    printErrNo();
    exit(EXIT_FAILURE);
  }

  scanTraceFile(traceFile);

  if (!fgets(line, 2048, traceFile)) {
    printErrNo();
    exit(EXIT_FAILURE);
  }

  sscanf(line, "%ld %ld %ld %ld", &ramSize, &virSize, &auSize, &pageSize);
  if (auSize > pageSize) pageSize = auSize;

  RAM = NULL; vir = NULL;
  RAM = new_Memory("./tmp/ep3.mem", ramSize);
  RAM->name = "Fisica";
  vir = new_Memory("./tmp/ep3.vir", virSize);
  vir->name = "Virtual";

  pageFrameCnt = getPageFrameCnt(ramSize);
  switch (pageSwapAlg) {
    case (1):
      filaFifo = (Block **)emalloc(pageFrameCnt*sizeof(Block *));
      for (ii = 0; ii < pageFrameCnt; ii++) filaFifo[ii] = NULL;
      filaFifoCnt = 0;
      break;
    case (2):
      filaFifo = (Block **)emalloc(pageFrameCnt*sizeof(Block *));
      for (ii = 0; ii < pageFrameCnt; ii++) filaFifo[ii] = NULL;
      filaFifoCnt = 0;
      break;
    case (3):
      lru2 = new_iMat(pageFrameCnt, pageFrameCnt);
      set_iMat(0, lru2, pageFrameCnt, pageFrameCnt); 
      break;
    case (4): 
      lru4 = (unsigned char *)emalloc(pageFrameCnt*sizeof(unsigned char));
      for (ii = 0; ii < pageFrameCnt; ii++) lru4[ii] = 0;
      break;
    default:
      break;
  }
  P = (Process *)emalloc(((size_t)Psize)*sizeof(Process));
  C = (unsigned *)emalloc(((size_t)Csize)*sizeof(unsigned));

  readTraceFileProcesses(traceFile);
  if (!RAM || !vir) {
    printErrNo();
    exit(EXIT_FAILURE);
  }
  fclose(traceFile);
}

typedef struct _lista {
  int tf;
  int size;
  struct _lista* next;
  struct _lista* previous;
} node;

node* create_node(int tf, int size, int *size_acc){
  node *temp = NULL;
  temp = (node *)emalloc(sizeof(node));
  temp->tf = tf;
  temp->size = size;
  temp->next = NULL;
  temp->previous = NULL;
  *size_acc = *size_acc + size;

  return temp;
}

void insert_node(node* n, node* head){
  node *temp;
  
  for (temp = head; temp->next != NULL; temp = temp->next);
  temp->next = n;
  n->previous = temp;
}

void remove_node(node **n, int *size_acc) {
  node *tmp;

  *size_acc = *size_acc - (*n)->size;

  if ((*n)->previous != NULL)
    (*n)->previous->next = (*n)->next;
  
  tmp = *n;
  *n = (*n)->next;
  if (*n != NULL)
    (*n)->previous = tmp->previous;

  free(tmp);
  tmp = NULL;
}

void random_trace() {
  int n = 128, procCnt = 0, t = 0, t_p = 0, size_acc = 0;
  int tf, size, total, virtual, Virtual, s, p, i, x, k;
  FILE *fp; char name[15]; node *head, *temp, *T;

  fp = fopen("./trace", "w");
  s = rand()%50 + 1;
  p = s*(rand()%20 + 1);
  total = p + rand()%5120;
  virtual = total + rand()%5120;

  head = create_node(-1, 0, &size_acc);

  fprintf(fp,"%d %d %d %d\n", total,virtual,s,p);
  Virtual = virtual - virtual%s;

  for (i = 0; procCnt < n; i++){
    if (rand()%10 == 1) {
       t += rand()%3;
      fprintf(fp, "%d COMPACTAR\n", t);
    }
    else {
      do {
        t += rand()%3;
        for (temp = head->next; temp != NULL;) {
          if (temp->tf >= 0 && temp->tf <= t)
            remove_node(&temp, &size_acc);
          else 
            temp = temp->next;
        }
      } while ((Virtual - size_acc) < s);

      sprintf(name, "proc-%d", procCnt);
      procCnt++; 
           
      while (1) {
        if ((Virtual - size_acc)%s != 0 || (Virtual - size_acc) == 0) {
          printErrNo();
          exit(1);
        }

        k = (Virtual - size_acc)/s;
        size = (1 + rand()%k)*s - rand()%s; 
        
        x = 0;

        if (size%s != 0) {
          x = s - size%s;
          size += x;
        }

        if (size > 0 && size >= s && size%s == 0) {
          tf = t + 1 + rand()%100;
          insert_node(create_node(tf, size, &size_acc), head);
          if (size_acc > Virtual) {
            printError(""); 
            exit(EXIT_FAILURE);
          }
          size -= x;
          break; 
        }
      }      
     
      fprintf(fp, "%d %d %d %s", t, tf, size, name);
      for (t_p = t; t_p < tf; t_p += rand()%5)
        fprintf(fp, " %d %d", rand()%size, t_p);
      fprintf(fp,"\n");
    }
  }

  for (temp = head; head != NULL && temp->next != NULL; temp = temp->next);
  
  for (; temp != NULL; ) {
    T = temp->previous;
  	free(temp);
    temp = NULL;
    temp = T;
  }

  fprintf(fp, "\n");
  fclose(fp);
}
