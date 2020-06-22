/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 * race_initialization.c: course initializations, frees and destroys
 */

#include "race.h"
#include "misc/error.h"
#include "misc/array.h"
#include "misc/thread.h"
#include <stdio.h>
#include <stdlib.h>

static void define_Course(Course *course, size_t lenght, size_t lapCnt, size_t cycCnt);
static void free_CycArr(Cyclist *cycArr);
static Cyclist *new_CycArr(size_t size);
static Course *new_Course(size_t lenght, size_t lapCnt, size_t cycCnt);
static void defineStartingPositions_Course(Course *course);
static void define_CycArr(Cyclist *cycArr, Course *course);
static void define_Cyc(Cyclist *cyc, size_t cycId, Course *course);

Course *init_Course(size_t lenght, size_t lapCnt, size_t cycCnt) {
  Course *course = NULL;

  course = new_Course(lenght, lapCnt, cycCnt);
  if (!course) return NULL;

  define_Course(course, lenght, lapCnt, cycCnt);

  return course;
}

static void define_Course(Course *course, size_t lenght, size_t lapCnt, size_t cycCnt) {
  size_t ii, jj;

  course->lenght = lenght;
  course->lapCnt = lapCnt;
  course->cycCnt = cycCnt;
  course->timeInterval_ms = 60;
  course->duration_ms = 0;
  course->frame = 0;
  course->mod = 2;
  course->lastLapDone = -1;
  course->isDone = 0;
  course->debug = 0;
  course->firstPlaceCyc = NULL;
  course->secondPlaceCyc = NULL;

  // course->buffer
  course->bufferCnt = 0;
  set_iArr(-1, course->buffer, cycCnt);
  sem_init(&course->buffer_mutex, 0, 1);

  // track positions
  for (ii = 0; ii < TRACK_WILDTH; ii++) 
    for (jj = 0; jj < lenght; jj++) {
      course->track[ii][jj] = -1;
      sem_init(&course->track_mutex[ii][jj], 0, 1);
    }

  // semaphores
  sem_init(&course->cycRunningCnt_sem, 0, (int)cycCnt);
  sem_init(&course->brokenCycCnt_sem, 0, 0);
  sem_init(&course->cycAt90_mutex, 0, 0); 
  for (ii = 0; ii < lapCnt; ii++)
    sem_init(&course->lapDoneCnt_sem[ii], 0, 0);
  
  // barriers
  pthread_barrier_init(&course->iteration_barrier1, NULL, (unsigned)cycCnt + 1);
  pthread_barrier_init(&course->iteration_barrier2, NULL, (unsigned)cycCnt + 1);
  
  // outputFile
  course->outputFile = stdout;

  define_CycArr(course->cycArr, course);
  defineStartingPositions_Course(course);
}

Cyclist ***new_CycPtrMat(size_t lineCnt, size_t collCnt) {
  Cyclist ***ptrMat;
  size_t line, coll;

  ptrMat = (Cyclist***)malloc(lineCnt*sizeof(Cyclist **));
  if (!ptrMat) {
    printErrNo(); 
    return NULL;
  }
  for (line = 0; line < lineCnt; line++) {
    ptrMat[line] = (Cyclist **)malloc(collCnt*sizeof(Cyclist *));
    if (!ptrMat[line]) {
      printErrNo();
      return NULL;
    } 
  }

  for (line = 0; line < lineCnt; line++)
    for (coll = 0; coll < collCnt; coll++)
      ptrMat[line][coll] = NULL;

  return ptrMat;
}

void free_CycPtrMat(Cyclist ***mat, size_t lineCnt) {
  size_t line = 0;

  for (line = 0; line < lineCnt; line++) {
    free(mat[line]);
    mat[line] = NULL;
  }
  free(mat);
  mat = NULL;
}

static Course *new_Course(size_t lenght, size_t lapCnt, size_t cycCnt) {
  Course *course = NULL;

  course = (Course *)malloc(sizeof(Course));
  if (!course) {
    printErrNo();
    goto error_noMem1;
  }
  course->lenght = lenght;
  course->lapCnt = lapCnt;
  course->track = new_iMat(TRACK_WILDTH, lenght);
  if (!course->track) goto error_noMem2;
  course->track_mutex = new_semMat(TRACK_WILDTH, lenght);
  if (!course->track_mutex) goto error_noMem3;
  course->lapDoneCnt_sem = new_semArr(lapCnt);
  if (!course->lapDoneCnt_sem) goto error_noMem4;
  course->cycArr = new_CycArr(cycCnt);
  if (!course->cycArr) goto error_noMem5;
  course->buffer = new_iArr(cycCnt);
  if (!course->buffer) goto error_noMem6;
  course->lapPos = new_CycPtrMat(lapCnt, cycCnt);
  if (!course->lapPos) return NULL;
  return course;


  // Error Management: free and set everything to NULL in case any allocation 
  //  fails;
  error_noMem6:
  free_CycArr(course->cycArr);
  error_noMem5:
  free_semArr(course->lapDoneCnt_sem);
  error_noMem4:
  free_semMat(course->track_mutex, TRACK_WILDTH);
  error_noMem3:
  free_iMat(course->track, TRACK_WILDTH);
  error_noMem2:
  free(course);
  error_noMem1:
  return (course = NULL);
}

void free_Course(Course *course) {
  size_t ii, jj;

  // free the track int 2d-array
  free_iMat(course->track, TRACK_WILDTH);
  free_iArr(course->buffer);
  sem_destroy(&course->buffer_mutex);
  // destroy and free the track sem_t 2d-array
  if (course->track_mutex) {
    for (ii = 0; ii < TRACK_WILDTH; ii++)
      for (jj = 0; jj < course->lenght; jj++)
        sem_destroy(&course->track_mutex[ii][jj]);  
    free_semMat(course->track_mutex, TRACK_WILDTH);
  }

  // destroy and free semaphores
  sem_destroy(&course->cycRunningCnt_sem);
  sem_destroy(&course->brokenCycCnt_sem);
  sem_destroy(&course->cycAt90_mutex); 
  if (course->lapDoneCnt_sem) {
    for (ii = 0; ii < course->lapCnt; ii++)
      sem_destroy(&course->lapDoneCnt_sem[ii]);
    free_semArr(course->lapDoneCnt_sem);
  }

  free_CycPtrMat(course->lapPos, course->lapCnt);

  // destroys p_thread_barrier_t
  pthread_barrier_destroy(&course->iteration_barrier1);
  pthread_barrier_destroy(&course->iteration_barrier2);

  free_CycArr(course->cycArr);

  // free the course pointer
  free(course);
  course = NULL;
}

static Cyclist *new_CycArr(size_t size) {
  Cyclist *arr = NULL;

  arr = (Cyclist *)malloc(size*sizeof(Cyclist));
  if (!arr) printErrNo();

  return arr;
}

static void defineStartingPositions_Course(Course *course) {
  sem_t **track_mutex;
  int **track;

  size_t cycCnt = course->cycCnt;
  size_t lenght =  course->lenght;
  track_mutex = course->track_mutex;
  track = course->track;

  for(size_t i = 0; i < cycCnt; i++) {
    size_t x = (lenght - (i/TRACK_WILDTH) - 1);
    size_t y = i%TRACK_WILDTH;
    course->cycArr[i].pos.x = x;
    course->cycArr[i].pos.y = y;
    sem_wait(&track_mutex[y][x]);
    track[y][x]=course->cycArr[i].id;
    course->cycArr[i].distTraveled = x - (int)course->lenght;
  }

  course->firstPlaceCyc = &course->cycArr[0];
  course->secondPlaceCyc = &course->cycArr[1];
}

static void define_CycArr(Cyclist *cycArr, Course *course) {
  for (size_t ii = 0; ii < course->cycCnt; ii++)
    define_Cyc(&cycArr[ii], ii, course);

  // defines a random cyc as capable of reaching 90 km/h
  int bestCyc;
  bestCyc = rand()%(int)course->cycCnt;
  cycArr[bestCyc].canReach90kmh = 1;
}

static void define_Cyc(Cyclist *cyc, size_t cycId, Course *course) {
  cyc->id = cycId;
  cyc->points = 0;
  cyc->pos.x = 0; 
  cyc->pos.y = 0;
  cyc->lap = -1;
  cyc->speed = 30;
  cyc->canReach90kmh = 0;
  cyc->isBroken = 0;
  cyc->isFinished = 0;
  cyc->finishTime_ms = -1;
  cyc->startedNewLap = 0;
  cyc->course = course;
  cyc->points_pos = 1;
  cyc->moved = 0;
  cyc->distTraveled = 0;
  cyc->plus20Cnt = 0;
}

static void free_CycArr(Cyclist *cycArr) {
  free(cycArr);
  cycArr = NULL;
}

