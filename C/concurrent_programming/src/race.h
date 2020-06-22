/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 */

#ifndef _race_h
#define _race_h
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define TRACK_WILDTH 10                // track wildth in meters
#define CYC_BREAK_CHANCE 1             // break percentage probability  
#define BREAK_CHANCE_MINIMUM_CYC_CNT 6 // cyclists can only break iff cyclist count >= this number
#define FINISHING_LAPS_CNT 2           // laps == (lapCnt - this number) proc events related to final laps
#define TRY_BREAK_AT_LAP_MOD 15        // laps such that lap%this == 0 imply a cyclist break chance
#define SPRINT_LAP 10
#define BEST_SPEED_CHANCE 10

typedef struct course_struct Course;
typedef struct position_struct Position;
typedef struct cyclist_struct Cyclist;

struct course_struct {
  size_t lenght;                        //
  size_t lapCnt;                        //
  size_t cycCnt;                        //
  Cyclist *cycArr;                      // courses's cyclists array
  int timeInterval_ms;                  // iterations's pseudo time interval
  int frame;
  int mod;

  int *buffer;
  int bufferCnt;
  sem_t buffer_mutex;
  
  int **track;                          // race's track
  sem_t **track_mutex;                  // mutex to control access to each of the track's position
  sem_t cycRunningCnt_sem;              // indicates how many are still running
  sem_t brokenCycCnt_sem;
  sem_t *lapDoneCnt_sem;                // indicates how many still need to finish each lap
  sem_t cycAt90_mutex;                  // indicates if a cyc has reached 90km/h
  int lastLapDone;                      // indicates the last lap completed by ALL cyclists
  Cyclist ***lapPos;                    // 2d-array to contain the cyclists's ptr in order of completion of each lap

  pthread_barrier_t iteration_barrier1; // barrier to make sure every cyclist moves before every new iteration
  pthread_barrier_t iteration_barrier2; // barrier to make sure every cyclist moves before every new iteration

  int duration_ms;                      // race duration
  char *outputFileName;                 // output file name
  FILE *outputFile;                     // file to contain the description of the race's events

  Cyclist *firstPlaceCyc;               // points to the first position cyclist, considering distance traveled
  Cyclist *secondPlaceCyc;              // points to the second position cyclist, considering distance traveled

  int isDone;                           // 1 iff race is done
  int debug;                            // 1 to print debug info during execution

} course_struct; 

struct position_struct {
  size_t x;  // horizontal axis
  size_t y;  // vertical axis
} position_struct;

struct cyclist_struct {
  int id;
  int points;
  Position pos;
  int speed;
  int canReach90kmh;
  int lap;
  int isBroken;
  int finishTime_ms;
  int isFinished;
  int startedNewLap;
  Course *course;
  int points_pos;
  int moved;
  int distTraveled;
  int plus20Cnt;
} cyclist_struct;

// race_initialization.c
Course *init_Course(size_t lenght, size_t lapCnt, size_t cycCnt);
void free_Course(Course *course);

// macros returned by updateLap()
#define COURSE_LAP_UPDATED 1
#define COURSE_LAP_NOT_UPDATED 0

// race.c
int flip(); //returns a random number from 0 to 99
void updateDuration_Course(Course *course);
void updateTimeInterval_Course(Course *course);
int updateLap_Course(Course *course);
void updateFrame_Course(Course *course);
void tryFinish_Course(Course *course);
void updateCycs_Course(Course *course);
void emptyBuffer(Course *course);

void move_Cyc(Cyclist *cyc);
void updateSpeed_Cyc(Cyclist *cyc);
void updateDist_Cyc(Cyclist *cyc);
void tryAwardPoints_Cyc(Cyclist *cyc);
void tryFinish_Cyc(Cyclist *cyc);
void tryBreak_Cyc(Cyclist *cyc);
void tryMove_Cyc(Cyclist *cyc);
void getFinishTime_Cyc(Cyclist *cyc);
void leaveTrack_Cyc(Cyclist* cyc);
void tryAwardPlus20Points(Course *course);
void awardPoints_CycArr(Cyclist *cycArr, size_t size);

void fprintTrack(Course *course, FILE *file);
void fprintFinalScoreBoard(Course *course, FILE *file);
void fprintScoreBoard(Course *course, FILE *file);
void fprint_race_order(Course *course, FILE *file);

#endif
