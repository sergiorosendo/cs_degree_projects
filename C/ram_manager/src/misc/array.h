/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#ifndef _array_h
#define _array_h
#include <stddef.h>

int *new_iArr(size_t size);
void free_iArr(int *arr);
int *resize_iArr(int *arr, size_t newSize);
void set_iArr(int x, int *arr, size_t size);
void copyTo_iArr(int *srcArr, int *destArr, size_t size);
void pull_iArr(int *arr, size_t index, size_t size);
void push_iArr(int *arr, size_t index, size_t size);
void print_iArr(int *arr, size_t size);
int **new_iMat(size_t rowCnt, size_t collCnt);
void free_iMat(int **mat, size_t rowCnt);
void set_iMat(int x, int **mat, size_t rowCnt, size_t collCnt);
void copyTo_iMat(int **srcMat, int **destMat, size_t rowCnt, size_t collCnt);

char *new_cArr(size_t size);
void free_cArr(char *arr);
char *resize_cArr(char *arr, size_t newSize);
void set_cArr(char c, char *arr, size_t size);
char **new_cMat(size_t rowCnt, size_t collCnt);
void free_cMat(char **mat, size_t rowCnt);
void set_cMat(char c, char **mat, size_t rowCnt, size_t collCnt);

#endif