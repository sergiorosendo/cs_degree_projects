/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 */

#include "thread.h"
#include "error.h"
#include <stdlib.h>

pthread_t *new_pthArr(size_t size) {
  pthread_t *arr = NULL;

  arr = (pthread_t *)malloc(size*sizeof(pthread_t));
  if (!arr) printErrNo();

  return arr;
}

void free_pthArr(pthread_t *arr) {
  free(arr);
  arr = NULL;
}

sem_t *new_semArr(size_t size) {
  sem_t *arr = NULL;

  arr = (sem_t *)malloc(size*sizeof(sem_t));
  if (!arr) printErrNo();

  return arr;
}

void free_semArr(sem_t *arr) {
  free(arr);
  arr = NULL;
}

sem_t **new_semMat(size_t lineCnt, size_t collCnt) {
  sem_t **mat;
  size_t line;

  mat = (sem_t **)malloc(lineCnt*sizeof(sem_t *));
  if (!mat) {
    printErrNo();
    return NULL;
  }
  for (line = 0; line < lineCnt; line++) {
    mat[line] = new_semArr(collCnt);
    if (!mat[line]) {
      free_semMat(mat, line);
      return NULL;
    }
  }

  return mat;
}

void free_semMat(sem_t **mat, size_t lineCnt) {
  size_t line;

  for (line = 0; line < lineCnt; line++)
    free_semArr(mat[line]);
  free(mat);
  mat = NULL;
}