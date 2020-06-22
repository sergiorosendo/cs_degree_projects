/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#include "array.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>

int *new_iArr(size_t size) {
  int *arr = NULL;

  arr = (int *)malloc(size*sizeof(int));
  if (!arr) {
    printErrNo();
    exit(EXIT_FAILURE);
  }

  return arr;
}

void free_iArr(int *arr) {
  free(arr);
  arr = NULL;
}

int *resize_iArr(int *arr, size_t newSize) {
  int *temp = NULL;

  temp = (int *)realloc(arr, newSize*sizeof(int));
  if (!temp)  {
    printErrNo();
    exit(EXIT_FAILURE);
  }

  return (arr = temp);
}

void set_iArr(int x, int *arr, size_t size) {
  size_t ii = 0;
  
  while (ii < size)
    arr[ii++] = x;
}

void copyTo_iArr(int *srcArr, int *destArr, size_t size) {
  for (size_t ii = 0; ii < size; ii++)
    destArr[ii] = srcArr[ii];
}

void pull_iArr(int *arr, size_t index, size_t size) {
  if (size == 0) return;

  for (size_t ii = index; ii < (size - 1); ii++)
    arr[ii] = arr[ii+1];
}

void push_iArr(int *arr, size_t index, size_t size) {
  if (size == 0) return;

  for (size_t ii = size - 1; ii > index; ii--)
    arr[ii] = arr[ii-1];
}

int **new_iMat(size_t rowCnt, size_t collCnt) {
  int **mat = NULL;

  mat = (int **)malloc(rowCnt*sizeof(int *));
  if (!mat)  {
    printErrNo();
    exit(EXIT_FAILURE);
  }
  
  for (size_t row = 0; row < rowCnt; row++) {
    mat[row] = new_iArr(collCnt);
    if (!mat[row]) {
      printErrNo();
      exit(EXIT_FAILURE);
    }
  }

  return mat;
}

void free_iMat(int **mat, size_t rowCnt) {
  size_t row = 0;

  while (row < rowCnt)
    free_iArr(mat[row++]);
  free(mat);
  mat = NULL;
}

void set_iMat(int x, int **mat, size_t rowCnt, size_t collCnt) {
  size_t row = 0;

  while (row < rowCnt)
    set_iArr(x, mat[row++], collCnt);
}

void copyTo_iMat(int **srcMat, int **destMat, size_t rowCnt, size_t collCnt) {
  for (size_t row = 0; row < rowCnt; row++)
    copyTo_iArr(srcMat[row], destMat[row], collCnt);
}

void print_iArr(int *arr, size_t size) {
  for (size_t ii = 0; ii < size; ii++)
    printf("%d\t", arr[ii]);
  printf("\n");
}

char *new_cArr(size_t size) {
  char *arr = NULL;

  arr = (char *)malloc(size*sizeof(char));
  if (!arr) {
    printErrNo();
    exit(EXIT_FAILURE);
  }
  
  return arr;
}

void free_cArr(char *arr) {
  free(arr);
  arr = NULL;
}

char *resize_cArr(char *arr, size_t newSize) {
  char *temp = NULL;

  temp = (char *)realloc(arr, newSize*sizeof(char));
  if (!temp) {
    printErrNo();
    exit(EXIT_FAILURE);
  }
  
  return (arr = temp);
}

void set_cArr(char c, char *arr, size_t size) {
  size_t ii = 0;

  while (ii < size)
    arr[ii++] = c;
}

char **new_cMat(size_t rowCnt, size_t collCnt) {
  char **mat = NULL;
  size_t row;

  mat = (char **)malloc(rowCnt*sizeof(char *));
  if (!mat) {
    printErrNo();
    exit(EXIT_FAILURE);
  }
  
  for (row = 0; row < rowCnt; row++) {
    mat[row] = new_cArr(collCnt);
    if (!mat[row]) {
      printErrNo();
      exit(EXIT_FAILURE);
    }
  }

  return mat;
}

void free_cMat(char **mat, size_t rowCnt) {
  size_t row = 0;

  while (row < rowCnt)
    free_cArr(mat[row++]);
  free(mat);
  mat = NULL;
}

void set_cMat(char c, char **mat, size_t rowCnt, size_t collCnt) {
  size_t row = 0;

  while (row < rowCnt)
    set_cArr(c, mat[row++], collCnt);
}