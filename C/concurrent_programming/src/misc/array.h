/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 */

#ifndef _array_h
#define _array_h
#include <stddef.h>

int *new_iArr(size_t size);
void free_iArr(int *arr);
int *resize_iArr(int *arr, size_t newSize);
void set_iArr(int x, int *arr, size_t size);
void pull_iArr(int *arr, size_t index, size_t size); //
void push_iArr(int *arr, size_t index, size_t size); //
int **new_iMat(size_t rowCnt, size_t collCnt);
void free_iMat(int **mat, size_t rowCnt);
void set_iMat(int x, int **mat, size_t rowCnt, size_t collCnt);

unsigned *new_uArr(size_t size);
void free_uArr(unsigned *arr);
unsigned *resize_uArr(unsigned *arr, size_t newSize);
void set_uArr(unsigned x, unsigned *arr, size_t size);
unsigned **new_uMat(size_t rowCnt, size_t collCnt);
void free_uMat(unsigned **mat, size_t rowCnt);
void set_uMat(unsigned x, unsigned **mat, size_t rowCnt, size_t collCnt);

char *new_cArr(size_t size);
void free_cArr(char *arr);
char *resize_cArr(char *arr, size_t newSize);
void set_cArr(char c, char *arr, size_t size);
char **new_cMat(size_t rowCnt, size_t collCnt);
void free_cMat(char **mat, size_t rowCnt);
void set_cMat(char c, char **mat, size_t rowCnt, size_t collCnt);

double *new_dArr(size_t size);
void free_dArr(double *arr);
double *resize_dArr(double *arr, size_t newSize);
void set_dArr(double x, double *arr, size_t size);
double **new_dMat(size_t rowCnt, size_t collCnt);
void free_dMat(double **mat, size_t rowCnt);
void set_dMat(double x, double **mat, size_t rowCnt, size_t collCnt);

float *new_fArr(size_t size);
void free_fArr(float *arr);
float *resize_fArr(float *arr, size_t newSize);
void set_fArr(float x, float *arr, size_t size);
float **new_fMat(size_t rowCnt, size_t collCnt);
void free_fMat(float **mat, size_t rowCnt);
void set_fMat(float x, float **mat, size_t rowCnt, size_t collCnt);

#endif