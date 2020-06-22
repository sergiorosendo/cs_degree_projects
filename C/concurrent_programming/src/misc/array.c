/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 */

#include "array.h"
#include "error.h"
#include <stdlib.h>

int *new_iArr(size_t size) {
  int *arr = NULL;

  arr = (int *)malloc(size*sizeof(int));
  if (!arr) printErrNo();

  return arr;
}

void free_iArr(int *arr) {
  free(arr);
  arr = NULL;
}

int *resize_iArr(int *arr, size_t newSize) {
  int *temp = NULL;

  temp = (int *)realloc(arr, newSize*sizeof(int));
  if (!temp) {
    printErrNo();
    free_iArr(arr);
  }

  return (arr = temp);
}

void set_iArr(int x, int *arr, size_t size) {
  size_t ii = 0;

  while (ii < size)
    arr[ii++] = x;
}

void pull_iArr(int *arr, size_t index, size_t size) {
  size_t ii;

  if (size == 0) return;
  for (ii = index; ii < (size - 1); ii++)
    arr[ii] = arr[ii+1];
}

void push_iArr(int *arr, size_t index, size_t size) {
  size_t ii;

  if (size == 0) return;
  for (ii = size - 1; ii > index; ii--)
    arr[ii] = arr[ii-1];
}

int **new_iMat(size_t rowCnt, size_t collCnt) {
  int **mat = NULL;
  size_t row;

  mat = (int **)malloc(rowCnt*sizeof(int *));
  if (!mat) {
    printErrNo();
    return NULL;
  }
  for (row = 0; row < rowCnt; row++) {
    mat[row] = new_iArr(collCnt);
    if (!mat[row]) {
      free_iMat(mat, row);
      return NULL;
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


unsigned *new_uArr(size_t size) {
  unsigned *arr = NULL;

  arr = (unsigned *)malloc(size*sizeof(unsigned));
  if (!arr) printErrNo();

  return arr;
}

void free_uArr(unsigned *arr) {
  free(arr);
  arr = NULL;
}

unsigned *resize_uArr(unsigned *arr, size_t newSize) {
  unsigned *temp = NULL;

  temp = (unsigned *)realloc(arr, newSize*sizeof(unsigned));
  if (!temp) {
    printErrNo();
    free_uArr(arr);
  }

  return (arr = temp);
}

void set_uArr(unsigned x, unsigned *arr, size_t size) {
  size_t ii = 0;

  while (ii < size)
    arr[ii++] = x;
}

unsigned **new_uMat(size_t rowCnt, size_t collCnt) {
  unsigned **mat = NULL;
  size_t row;

  mat = (unsigned **)malloc(rowCnt*sizeof(unsigned *));
  if (!mat) {
    printErrNo();
    return NULL;
  }
  for (row = 0; row < rowCnt; row++) {
    mat[row] = new_uArr(collCnt);
    if (!mat[row]) {
      free_uMat(mat, row);
      return NULL;
    }
  }

  return mat;
}

void free_uMat(unsigned **mat, size_t rowCnt) {
  size_t row = 0;

  while (row < rowCnt)
    free_uArr(mat[row++]);
  free(mat);
  mat = NULL;
}

void set_uMat(unsigned x, unsigned **mat, size_t rowCnt, size_t collCnt) {
  size_t row = 0;

  while (row < rowCnt)
    set_uArr(x, mat[row++], collCnt);
}

char *new_cArr(size_t size) {
  char *arr = NULL;

  arr = (char *)malloc(size*sizeof(char));
  if (!arr) printErrNo();

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
    free_cArr(arr);
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
    return NULL;
  }
  for (row = 0; row < rowCnt; row++) {
    mat[row] = new_cArr(collCnt);
    if (!mat[row]) {
      free_cMat(mat, row);
      return NULL;
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


double *new_dArr(size_t size) {
  double *arr = NULL;

  arr = (double *)malloc(size*sizeof(double));
  if (!arr) printErrNo();

  return arr;
}

void free_dArr(double *arr) {
  free(arr);
  arr = NULL;
}

double *resize_dArr(double *arr, size_t newSize) {
  double *temp = NULL;

  temp = (double *)realloc(arr, newSize*sizeof(double));
  if (!temp) {
    printErrNo();
    free_dArr(arr);
  }

  return (arr = temp);
}

void set_dArr(double x, double *arr, size_t size) {
  size_t ii = 0;

  while (ii < size)
    arr[ii++] = x;
}

double **new_dMat(size_t rowCnt, size_t collCnt) {
  double **mat = NULL;
  size_t row;

  mat = (double **)malloc(rowCnt*sizeof(double *));
  if (!mat) {
    printErrNo();
    return NULL;
  }
  for (row = 0; row < rowCnt; row++) {
    mat[row] = new_dArr(collCnt);
    if (!mat[row]) {
      free_dMat(mat, row);
      return NULL;
    }
  }

  return mat;
}

void free_dMat(double **mat, size_t rowCnt) {
  size_t row = 0;

  while (row < rowCnt)
    free_dArr(mat[row++]);
  free(mat);
  mat = NULL;
}

void set_dMat(double x, double **mat, size_t rowCnt, size_t collCnt) {
  size_t row = 0;

  while (row < rowCnt)
    set_dArr(x, mat[row++], collCnt);
}


float *new_fArr(size_t size) {
  float *arr = NULL;

  arr = (float *)malloc(size*sizeof(float));
  if (!arr) printErrNo();

  return arr;
}

void free_fArr(float *arr) {
  free(arr);
  arr = NULL;
}

float *resize_fArr(float *arr, size_t newSize) {
  float *temp = NULL;

  temp = (float *)realloc(arr, newSize*sizeof(float));
  if (!temp) {
    printErrNo();
    free_fArr(arr);
  }

  return (arr = temp);
}

void set_fArr(float x, float *arr, size_t size) {
  size_t ii = 0;

  while (ii < size)
    arr[ii++] = x;
}

float **new_fMat(size_t rowCnt, size_t collCnt) {
  float **mat = NULL;
  size_t row;

  mat = (float **)malloc(rowCnt*sizeof(float *));
  if (!mat) {
    printErrNo();
    return NULL;
  }
  for (row = 0; row < rowCnt; row++) {
    mat[row] = new_fArr(collCnt);
    if (!mat[row]) {
      free_fMat(mat, row);
      return NULL;
    }
  }

  return mat;
}

void free_fMat(float **mat, size_t rowCnt) {
  size_t row = 0;

  while (row < rowCnt)
    free_fArr(mat[row++]);
  free(mat);
  mat = NULL;
}

void set_fMat(float x, float **mat, size_t rowCnt, size_t collCnt) {
  size_t row = 0;

  while (row < rowCnt)
    set_fArr(x, mat[row++], collCnt);
}