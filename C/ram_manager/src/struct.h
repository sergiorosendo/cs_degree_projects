/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#ifndef _struct_h
#define _struct_h

#include <stddef.h>
#include <stdio.h>

typedef struct process_struct Process;
typedef struct position_acess_node_struct PosAcess;
typedef struct memory_block_node_struct Block;
typedef struct memory_struct Memory;
typedef struct page_struct Page;
typedef struct hist_struct Hist;

struct hist_struct{
	int cnt;
	ssize_t size;
};


struct memory_struct {
  FILE *mem;
  Block *block;
  int *bit_map;
  ssize_t size;
  char *name;
} memory_struct;

struct memory_block_node_struct {
  ssize_t pos;                //starting allocation unit id
  ssize_t size;               //block's size (in number allocation units)
  ssize_t absSize;            //block's number of occupied bytes
  Process *proc;              //block's owning process
  Page *page;                 //block's owning process page
  Block *next;                
  Block *previous;
  Memory *M;
  int next_access;
  Block* nextQuick1;
  Block* nextQuick2;  
} memory_block_node_struct;

struct position_acess_node_struct {
  ssize_t local_pos;
  unsigned t;
  PosAcess *next;
} position_acess_node_struct;

struct page_struct {
  ssize_t id;
  int lastPage;
  Process *proc;
  int inRAM;
  Block *block;
} page_struct;

struct process_struct {
  unsigned t0;
  unsigned tf;
  ssize_t b;
  char *name;
  char PID;
  PosAcess *A;
  Block *block;
  Page *page;
  int isFinished;
} process_struct;

//Global variables
extern Memory *RAM;          //physical memory
extern Memory *vir;          //virtual memory
extern unsigned Time;        //pseudo Time passed in seconds
extern Process *P;           //process array
extern ssize_t Psize;        //process array size
extern unsigned *C;          //compress Time array
extern ssize_t Csize;        //compress array size
extern unsigned pfCnt;       //page faults counter
extern ssize_t auSize;       //allocation unit size
extern ssize_t pageSize;     //page size
extern ssize_t compressCnt;  //memory compressions done cnt
extern int freeSpaceAlg;     //free space manager algorithm id
extern int pageSwapAlg;      //page swap manager algorithm
extern unsigned interval;    //interval in seconds to print memories
extern int **lru2;           //lru2 matrix
extern unsigned char *lru4;  //lru4 R array
extern ssize_t pageFrameCnt; //RAM number of pageFrameCnt
extern Block** filaFifo;
extern int filaFifoCnt; 
extern Hist* quickProcHist;
extern int histCnt;
extern ssize_t quickSize1;
extern ssize_t quickSize2;
extern Block* quickList1;
extern Block* quickList2;
#endif
