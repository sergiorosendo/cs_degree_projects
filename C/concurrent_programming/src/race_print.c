/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 */

#include "race.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int pts_order(const void* c1, const void* c2);
int race_order(const void* c1, const void* c2);
static char* print_cell(int t, int n);
char* format_time(int milisecs);

int race_order(const void* c1, const void* c2){

  Cyclist* a = *((Cyclist**) c1);
  Cyclist* b = *((Cyclist**) c2);
  if (a->isFinished && a->isBroken == 0) { //se a ja terminou
    if (b->isFinished && b->isBroken == 0) { // se b tb terminou
      if (a->finishTime_ms < b->finishTime_ms) //a<b
        return -1;                                                                              
      else
        return 1;
    }
    else // se b nao terminou a<b
      return -1;
  }
  else {           // se a nao terminou
    if (b->isFinished && b->isBroken == 0) // se b terminou
      return 1;   //b<a
    else {
      if (a->isBroken) { //se a quebrou
        if (b->isBroken) { // se b quebrou
          if (a->finishTime_ms < b->finishTime_ms) // maior race_time eh menor
            return 1;
          else
            return -1;
        }
        else //se b nao quebro
          return 1; //b<a
      }
      else { //a e b nao terminaram, e a nao quebro
        if (b->isBroken) // se b quebro
          return -1; //a<b
        else { // ninguem termino nem quebro
          size_t d1 = a->distTraveled;
          size_t d2 = b->distTraveled;
          if (d1 < d2)
            return 1;
          else
            return -1;
        }
      }
    }
  }
}

void fprint_race_order(Course *course, FILE *file){

 // size_t n = course->cycCnt;
 // char *formated;
 // Cyclist **temp = (Cyclist**)malloc(n*sizeof(Cyclist*));
  size_t ii;
  if (course->lastLapDone == -1) return;
  fprintf(file, "\nPosições (volta %d):\n", course->lastLapDone + 1);
  for (ii = 0; (ii < course->cycCnt) && (course->lapPos[course->lastLapDone][ii] != NULL); ii++)
    fprintf(file, "%lu)\tCic-%d\n", ii + 1, course->lapPos[course->lastLapDone][ii]->id);
  for (ii = 0; ii < course->cycCnt; ii++) {
    if (course->cycArr[ii].isBroken == 1 && course->cycArr[ii].lap <= course->lastLapDone)
      fprintf(file, "*)\tCic-%d\tQuebrou\n", course->cycArr[ii].id);
  }
  
  fprintf(file, "\n");
/*  for (size_t i = 0; i < n; i++)
    temp[i] = &course->cycArr[i];
  qsort(temp,n,sizeof(Cyclist*), race_order);
  
  for (size_t i=0;i<n;i++) {
    if ((temp[i]->isFinished == 1) && (temp[i]->isBroken == 0)) {
      formated = format_time(temp[i]->finishTime_ms);
      fprintf(file,"%lu)\tCic-%d\tTempo: %s\n", i, temp[i]->id, formated);
      free(formated);
    }
    else {
      if (temp[i]->isBroken)
        fprintf(file,"*)\tCic-%d\tQuebrou na volta %d\n", temp[i]->id, temp[i]->lap + 1);
      else
        fprintf(file,"%lu)\tCic-%d\n", i, temp[i]->id);
    }*
  }
  free(temp);*/
}

static int pts_order(const void* c1, const void* c2) {

  Cyclist* a = *((Cyclist**) c1);
  Cyclist* b = *((Cyclist**) c2);
  if (a->isBroken) { //se a ja terminou
    if (b->isBroken) { // se b tb terminou
      if (a->lap < b->lap)
        return 1;
      else
        return -1;
      }
    else
      return 1;
  }
  else {
    if (b->isBroken)
      return -1;
    else {
      if (a->points < b->points)
        return 1;
      else
        return -1;
    }
  }
}

void fprintScoreBoard(Course *course, FILE *file){
  size_t n = course->cycCnt;
  Cyclist **temp = (Cyclist**) malloc(n*sizeof(Cyclist*));
  for (size_t i = 0; i < n; i++)
    temp[i] = &course->cycArr[i];

  qsort(temp, n, sizeof(Cyclist*), pts_order);
  
  fprintf(file, "\nPlacar (volta %d):\n", course->lastLapDone + 1);
  for (size_t i=0;i<n;i++) {
    temp[i]->points_pos = i + 1;
    if (temp[i]->isBroken)
      fprintf(file, "*)\tCic-%d\tQuebrou\n", temp[i]->id);
    else {
      fprintf(file, "%lu)\tCic-%d\t%d Pts\n", i+1, temp[i]->id, temp[i]->points);
    }
  }
  fprintf (file, "\n");
  free(temp);
}

char* format_time(int milisecs) {
	char* formated = (char*) malloc(30*sizeof(char));
	int ml = milisecs%1000;
	int secs = (milisecs/1000)%60;
	int min = (milisecs/60000)%60;
	int hour = milisecs/3600000;
	sprintf(formated,"%dh %dmin %ds %dms",hour, min, secs, ml);
	return formated;

}
void fprintFinalScoreBoard(Course *course, FILE *file) {
  
  char* formated;
  size_t n = course->cycCnt;
  Cyclist **temp = (Cyclist**)malloc(n*sizeof(Cyclist*));
  
  for (size_t i = 0; i < n; i++)
    temp[i] = &course->cycArr[i];

  qsort(temp , n, sizeof(Cyclist*), pts_order);
  
  fprintf(file, "Placar Final:\n");
  for (size_t i = 0; i < n; i++) {
    if (temp[i]->isBroken)
      fprintf(file, "*)\tCic-%d\t\t%d Pts\t\tQuebrou na volta %d\n", temp[i]->id, temp[i]->points, temp[i]->lap+1);
    else{
      formated = format_time(temp[i]->finishTime_ms);
      fprintf(file, "%lu)\tCic-%d\t\t%d Pts\t\tTempo: %s\n", i+1, temp[i]->id, temp[i]->points, formated);
      free(formated);
    }
  }

  free(temp);

}

static char* print_cell(int t, int n) {
      
  char temp[5];
  sprintf(temp, "%d", n - 1);
  int s = strlen(temp);  
  char* a = (char*)malloc((s + 1)*sizeof(char));
  if (t < 0) {
    for (int i = 0; i < s; i++)
      a[i] = ' ';
    a[s] = '\0';
  }
  else {
    char temp[s+1];
    sprintf(temp,"%d",t);
    int k = strlen(temp);
    int i = 0, j = 0;
    while (i<(s-k)/2)
      a[i++] = ' ';
    while (j<k)
      a[i++] = temp[j++];
    while (i < s)
      a[i++] = ' ';
    a[i] = '\0';
  }
  return a;
}

void fprintTrack(Course *course, FILE *file) {
  
  int empty;
  
  char *cTime;
  cTime = format_time(course->duration_ms);
  fprintf(file, "Pista (t: %s):\n", cTime);
  free(cTime);

  for (size_t i = 0; i < course->lenght; i++) {
    empty = 0;
    for (size_t j = 0; j < TRACK_WILDTH; j++) {
      if (course->track[j][course->lenght-i-1] !=-1){
	   char* text = print_cell(course->track[j][course->lenght -i -1], course->cycCnt);
           fprintf(file, "|%s", text );
           free(text);
      }
      else
        empty++;
    }
    for (int j = 0; j < empty; j++){		   
      char* text = print_cell(-1 ,course->cycCnt);
      fprintf(file, "|%s", text );
      free(text);
    }       
    fprintf(file, "|\n");
  }
  fprintf(file, "\n\n");
} 
