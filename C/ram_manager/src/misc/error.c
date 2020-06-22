/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#include "error.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RESETCOLOR   "\033[0m"           // 
#define BLACK        "\033[30m"          // Black
#define RED          "\033[31m"          // Red
#define GREEN        "\033[32m"          // Green
#define YELLOW       "\033[33m"          // Yellow
#define BLUE         "\033[34m"          // Blue
#define MAGENTA      "\033[35m"          // Magenta
#define CYAN         "\033[36m"          // Cyan
#define WHITE        "\033[37m"          // White 
#define BOLDBLACK    "\033[1m\033[30m"   // Bold Black
#define BOLDRED      "\033[1m\033[31m"   // Bold Red
#define BOLDGREEN    "\033[1m\033[32m"   // Bold Green
#define BOLDYELLOW   "\033[1m\033[33m"   // Bold Yellow
#define BOLDBLUE     "\033[1m\033[34m"   // Bold Blue 
#define BOLDMAGENTA  "\033[1m\033[35m"   // Bold Magenta
#define BOLDCYAN     "\033[1m\033[36m"   // Bold Cyan
#define BOLDWHITE    "\033[1m\033[37m"   // Bold White

static void pError_label();
static void pWarning_label();
static void pFnName_label(const char funcName[]);
static void pSrcFileName_label(const char srcFileName[]);
static void pSrcLine_label(int srcLine);
static void pFunction_info(const char funcName[], const char srcFileName[], int srcLine);

void pErrNo(const char funcName[], const char srcFileName[], int srcLine) {
  pFunction_info(funcName, srcFileName, srcLine);
  pError_label();
  fprintf(stderr, " %s.\n", strerror(errno));
}

void pError(const char errorMsg[], const char funcName[], const char srcFileName[], int srcLine) {
  pFunction_info(funcName, srcFileName, srcLine);
  pError_label();
  fprintf(stderr, " %s.\n", errorMsg);
}

void pWarning(const char warningMsg[], const char funcName[], const char srcFileName[], int srcLine) {
  pFunction_info(funcName, srcFileName, srcLine);
  pWarning_label();
  fprintf(stderr, " %s.\n", warningMsg);
}


static void pError_label() {
  fprintf(stderr, BOLDRED "error:" RESETCOLOR);
}

static void pWarning_label() {
  fprintf(stderr, BOLDMAGENTA "warning:" RESETCOLOR);
}

static void pFnName_label(const char funcName[]) {
  fprintf(stderr, BOLDWHITE "'%s'" RESETCOLOR, funcName);
}

static void pSrcFileName_label(const char srcFileName[]) {
  fprintf(stderr, BOLDWHITE "%s:" RESETCOLOR, srcFileName);
}

static void pSrcLine_label(int srcLine) {
  fprintf(stderr, BOLDWHITE "%d:" RESETCOLOR, srcLine);
}

static void pFunction_info(const char funcName[], const char srcFileName[], int srcLine) {
  pSrcFileName_label(srcFileName);
  fprintf(stderr, " In function ");
  pFnName_label(funcName);
  fprintf(stderr, ":\n");
  pSrcFileName_label(srcFileName);
  pSrcLine_label(srcLine);
  fprintf(stderr, " ");
}

void *emalloc(size_t size) {
  void *m = malloc(size);
  
  if (!m) {
    printErrNo();
    exit(EXIT_FAILURE);
  }

  return m;
}