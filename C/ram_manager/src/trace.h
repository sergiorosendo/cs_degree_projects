/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP3 - MAC0422: Sistemas Operacionais - 20/11/2017 
 */

#ifndef _trace_h
#define _trace_h

#include "struct.h"
#include <stddef.h>

void readPosAcess(char *token, Process *proc);
void readProcess(char *line, Process *proc, int PID);
void readTraceFileProcesses(FILE *traceFile);
void scanTraceFile(FILE *traceFile);
void readTraceFile(const char traceFileName[]);
void random_trace();
int ordena_hist(const void* a, const void* b);
#endif
