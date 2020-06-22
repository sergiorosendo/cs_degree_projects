/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 */

#ifndef _thread_h
#define _thread_h
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>

pthread_t *new_pthArr(size_t size);
void free_pthArr(pthread_t *arr);

sem_t *new_semArr(size_t size);
void free_semArr(sem_t *arr);
sem_t **new_semMat(size_t lineCnt, size_t collCnt);
// returns a pointer to a malloced 2-dimension sem_t array 'mat[lineCnt][collCnt]'
void free_semMat(sem_t **mat, size_t lineCnt);

#endif