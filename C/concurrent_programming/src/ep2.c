/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 * ep2.c: main, race simulation and thread functions
 */

#include "race.h"
#include "misc/error.h"
#include "misc/array.h"
#include "misc/thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

void *th_coord(void *arg) {
  Course *course;
  int cycRunningCnt = 0;
  int event = 0;

  course = (Course *)arg;
  sem_getvalue(&course->cycRunningCnt_sem, &cycRunningCnt);

  while (!course->isDone) {
    // waits for cyclist to finish moving
    pthread_barrier_wait(&course->iteration_barrier1);
    // COORDINATOR WORK HERE

    emptyBuffer(course);
    updateCycs_Course(course);

    // reinitiates barrier1
    sem_getvalue(&course->cycRunningCnt_sem, &cycRunningCnt);
    pthread_barrier_destroy(&course->iteration_barrier1);
    pthread_barrier_init(&course->iteration_barrier1, NULL, (unsigned)cycRunningCnt + 1); 

    if (course->debug == 1) 
      fprintTrack(course, stderr);

    updateDuration_Course(course);
    event = updateLap_Course(course);

    updateTimeInterval_Course(course); 
    if (event == COURSE_LAP_UPDATED) { 
      fprint_race_order(course, course->outputFile);
      if ((course->lastLapDone > 2) && (course->lastLapDone + 1)%10 == 0)
	      fprintScoreBoard(course, stderr);
      tryFinish_Course(course);
    }

    updateFrame_Course(course);

    // waits for cyclists to reach barrier2
    pthread_barrier_wait(&course->iteration_barrier2);
    // reinitiates barrier2
    pthread_barrier_destroy(&course->iteration_barrier2);
    pthread_barrier_init(&course->iteration_barrier2, NULL, (unsigned)cycRunningCnt + 1);
  }

  fprintFinalScoreBoard(course, stderr);

  return NULL;
}

void *th_cyclist(void *arg) {
  // Cyclist thread function
  Cyclist *cyc;

  cyc = (Cyclist *)arg;

  // Cyclist loop
  while (cyc->finishTime_ms == -1) {
    // Cyclist works
    move_Cyc(cyc);

    pthread_barrier_wait(&cyc->course->iteration_barrier1);
    // Cyclist waits for coordinator thread to finish
    pthread_barrier_wait(&cyc->course->iteration_barrier2);
  }
  return NULL;
}

int simulateRace(size_t trackLenght, size_t lapCnt, size_t cycCnt, int debug) {
  Course *course = NULL;  
  pthread_t *thCycArr = NULL; // cyclists thread array
  pthread_t thCoord;          // coordinator thread

  // memory allocation
  course = init_Course(trackLenght, lapCnt, cycCnt);
  if (!course) goto error_noMem1;
  if (debug == 1) course->debug = 1;
  thCycArr = new_pthArr(cycCnt);
  if (!thCycArr) goto error_noMem2;

  // executes the coordinator thread
  if (pthread_create(&thCoord, NULL, th_coord, (void *)course) != 0) {
      printErrNo();
      goto error_thCreateFail;
  }
  // executes each cyclist thread

  for (size_t ii = 0; ii < cycCnt; ii++)
    if (pthread_create(&thCycArr[ii], NULL, th_cyclist, (void *)&course->cycArr[ii]) != 0) {
      printErrNo();
      goto error_thCreateFail;
    }
  
  // Waits for all threads to finish
  for (size_t ii = 0; ii < course->cycCnt; ii++) 
    pthread_join(thCycArr[ii], NULL); 
  pthread_join(thCoord, NULL); 

  // final frees
  free_pthArr(thCycArr);
  free_Course(course);
  return 0;  

  // error treatment
  error_thCreateFail: // could not create a cyclist thread
  free_pthArr(thCycArr);
  error_noMem2:       // could not allocate memory for cyclist array
  free_Course(course);
  error_noMem1:       // could not allocate memory for course
  return -1;
}

int main(int argc, char **argv) 
{
  int debug = 0, ii;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <speedway_lenght> <number_of_laps> <number_of_cyclists> [d]\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  for (ii = 1; ii < argc; ii++)
    if (strcmp(argv[ii], "d") == 0) 
      debug = 1;

  srand (time(NULL));
  simulateRace(atoi(argv[1]), atoi(argv[3]), atoi(argv[2]), debug);
  
  return EXIT_SUCCESS;
}
