/* Guilherme Feulo do Espirito Santo - 6431749
 * Sergio Rosendo da Silva Junior    - 6508702
 *
 * EP2 - MAC0422: Sistemas Operacionais - 16/10/2017 
 *
 * race.c: course and cyclists management functions
 */

#include "race.h"
#include "misc/error.h"
#include "misc/array.h"
#include "misc/thread.h"
#include <stdio.h>
#include <stdlib.h>

int flip() {
  return rand()%100;
}

void updateDuration_Course(Course *course) {
  course->duration_ms += course->timeInterval_ms;
}

void updateFrame_Course(Course *course) {
  course->frame = (course->frame + 1)%course->mod;
}

void updateDist_Course(Course *course) {
  for (size_t ii = 0; ii < course->cycCnt; ii++)
    if (course->cycArr[ii].moved == 1) {
      course->cycArr[ii].moved = 0;
      updateDist_Cyc(&course->cycArr[ii]);
    }
}

void updateDist_Cyc(Cyclist *cyc) { 
  cyc->distTraveled += 1;
  if (cyc->distTraveled > cyc->course->secondPlaceCyc->distTraveled)
    if (cyc != cyc->course->firstPlaceCyc) {
      cyc->course->secondPlaceCyc = cyc;
      if (cyc->course->secondPlaceCyc->distTraveled > cyc->course->firstPlaceCyc->distTraveled) {
        cyc->course->secondPlaceCyc = cyc->course->firstPlaceCyc;
        cyc->course->firstPlaceCyc = cyc;
      }
    }
}

void updateTimeInterval_Course(Course *course) {
  if (course->lastLapDone == -1) return;
  // maintains interval if it already is 20 
  if (course->timeInterval_ms == 20) return;

  int cycAt90 = 0;
  sem_getvalue(&course->cycAt90_mutex, &cycAt90);
  // if there's a cyclist at 90km/h updates interval
  if (cycAt90 == 1) {
    if(course->frame ==1)
	    course->frame=3;
    course->timeInterval_ms = 20;
    course->mod = 6;
    //fprintf(stderr,"troco o mod para 6\n");
  }
} 

int updateLap_Course(Course *course) {
  int lapDoneCnt = -1;

  sem_getvalue(&course->lapDoneCnt_sem[course->lastLapDone+1], &lapDoneCnt);
  if (lapDoneCnt == (int)course->cycCnt) {
    course->lastLapDone += 1;
    return COURSE_LAP_UPDATED;
  }
  return COURSE_LAP_NOT_UPDATED;
} 

void tryFinish_Course(Course *course) {
  if ((size_t)course->lastLapDone >= course->lapCnt - 1) 
    course->isDone = 1;
}


void move_Cyc(Cyclist *cyc) {

  cyc->moved = 0;
  cyc->startedNewLap = 0;
  tryMove_Cyc(cyc);

  if (cyc->moved == 1) {
    if (cyc->pos.x == 0) {
    // cyc just started a new lap
      cyc->lap++;
      if (cyc->lap <= 0) return;
      cyc->startedNewLap = 1;
      return;
    }
    return;
  }

  // cyc not not change its position in course->track
  return;
}

void updateSpeed_Cyc(Cyclist *cyc) {
  if (cyc->speed == 90) return;

  if ((cyc->canReach90kmh == 1) && cyc->lap >= ((int)cyc->course->lapCnt - FINISHING_LAPS_CNT - 1))
    if (flip() < BEST_SPEED_CHANCE) {
      sem_post(&cyc->course->cycAt90_mutex);
      cyc->speed = 90;
      return;
    }

  if (cyc->speed == 30) {
    if (flip() < 70)
      cyc->speed = 60;
    return;
  }
  else {
    if (cyc->speed == 60) {
      if (flip() < 50)
        cyc->speed = 30;
    }
    return;
  }
}

void tryAwardPoints_Cyc(Cyclist *cyc) {
  int brokenCycCnt;
  int lapDoneCnt;
  int posRank;
  size_t ii;

  sem_post(&cyc->course->lapDoneCnt_sem[cyc->lap-1]);
  
  for (ii = 0; cyc->course->lapPos[cyc->lap-1][ii] != NULL; ii++);
  cyc->course->lapPos[cyc->lap-1][ii] = cyc;

  if (cyc->lap < 1) return;

  if ((cyc->lap)%SPRINT_LAP != 0) return;

  sem_getvalue(&cyc->course->brokenCycCnt_sem, &brokenCycCnt);
  sem_getvalue(&cyc->course->lapDoneCnt_sem[cyc->lap-1], &lapDoneCnt);
  posRank = lapDoneCnt - brokenCycCnt;

  switch (posRank) {
    case (1):
      cyc->points += 5; 
      break;
    case (2):
      cyc->points += 3; break;
    case (3):
      cyc->points += 2; break;
    case (4):
      cyc->points += 1; break;
    default:
      break;
  }
}

void tryFinish_Cyc(Cyclist *cyc) {
  if ((size_t)cyc->lap >= cyc->course->lapCnt) {
    leaveTrack_Cyc(cyc);
    getFinishTime_Cyc(cyc);
    sem_wait(&cyc->course->cycRunningCnt_sem);
  }
}

void tryBreak_Cyc(Cyclist *cyc) {
  int cycRunningCnt = 0;

  if (cyc->startedNewLap == 0) // can break only when starting a new lap
    return; 
  if (cyc->lap <= 1) //cyc just started the racprinte
    return;
  if ((cyc->isFinished == 1) || ((size_t)cyc->lap >= cyc->course->lapCnt)) //cyc already finished the race
    return; 
  if ((cyc->lap)%15 != 0) //can only break when finishing laps multiples of 15
    return;

  sem_getvalue(&cyc->course->cycRunningCnt_sem, &cycRunningCnt);
  if (cycRunningCnt < BREAK_CHANCE_MINIMUM_CYC_CNT) //too few cyclists left 
    return; 
  
  if (flip() < CYC_BREAK_CHANCE) {
    sem_wait(&cyc->course->cycRunningCnt_sem);
    leaveTrack_Cyc(cyc);
    getFinishTime_Cyc(cyc);
    cyc->isBroken = 1;
    sem_post(&cyc->course->brokenCycCnt_sem);
    for (size_t ii = (size_t)cyc->lap; ii < cyc->course->lapCnt; ii++)
      sem_post(&cyc->course->lapDoneCnt_sem[ii]); //tells the course this cyclist "finished" all laps
    fprintf(stderr, "Cic-%d quebrou na volta %d, estava em %dº"
      " na classificação por pontos.\n", cyc->id, cyc->lap + 1, cyc->points_pos);
  }
}

void tryMove_Cyc(Cyclist *cyc) {
  int frame = cyc->course->frame;
  int mod = cyc->course->mod;

  int lenght = cyc->course->lenght;
  int pos = cyc->pos.x;
  int lane = cyc->pos.y;
  int n_pos = (pos+1)%lenght;

  int** track = cyc->course->track;
  sem_t** track_mutex = cyc->course->track_mutex;
  
  if (frame != 0) {
    if ((cyc->speed == 60 && frame%(mod/2) == 0) || ((cyc->speed == 90) && frame%(mod/3) == 0)) {
        int n_lane = -1;
        for (int i = 0; i < TRACK_WILDTH; i++){
          if (!sem_trywait(&track_mutex[i][n_pos])){
            n_lane = i;
            break;
          }
        }
      if (n_lane >= 0) {
        cyc->pos.x = n_pos;
        cyc->pos.y = n_lane;
        track[n_lane][n_pos] = cyc->id;
        track[lane][pos] = -1;
        sem_post(&track_mutex[lane][pos]);
        cyc->moved = 1;
      }
      else {
       	sem_wait(&cyc->course->buffer_mutex);
	      cyc->course->buffer[cyc->course->bufferCnt] = cyc->id;
	      cyc->course->bufferCnt++;
	      sem_post(&cyc->course->buffer_mutex);     		
      }
    }
  }
  else {
    track[lane][pos] = -1;
    sem_post(&track_mutex[lane][pos]);
    sem_wait(&track_mutex[lane][n_pos]);
    track[lane][n_pos] = cyc->id;
    cyc->pos.x = n_pos;
    cyc->moved = 1;
  }  
}

void getFinishTime_Cyc(Cyclist *cyc) {
  cyc->finishTime_ms = cyc->course->duration_ms + cyc->course->timeInterval_ms;
  cyc->isFinished = 1;
}

void leaveTrack_Cyc(Cyclist* cyc) {
  cyc->course->track[cyc->pos.y][cyc->pos.x] = -1;
  sem_post(&(cyc->course->track_mutex[cyc->pos.y][cyc->pos.x]));
}

void emptyBuffer(Course *course) {
	Cyclist* temp;
	int n_pos,j; 
	int **track = course->track;
	sem_t **track_s = course->track_mutex;
	for (int i = 0; i < course->bufferCnt; i++){

		temp = &course->cycArr[course->buffer[i]];

		n_pos = (temp->pos.x + 1)%course->lenght;
		for (j = 0; j < TRACK_WILDTH; j++){
			if (track[j][n_pos] == -1)
				break;
			
		}
		if (j < TRACK_WILDTH) {

			if (!sem_trywait(&track_s[j][n_pos])){
				track[temp->pos.y][temp->pos.x] =-1;
				sem_post(&track_s[temp->pos.y][temp->pos.x]);
				track[j][n_pos] = temp->id;
				temp->pos.x = n_pos;
				temp->pos.y = j;	
			}
		}
		course->buffer[i]=-1;	
	}
	course->bufferCnt=0;
}

void awardPoints_CycArr(Cyclist *cycArr, size_t size) {
  for (size_t ii = 0; ii < size; ii++)
    if (cycArr[ii].startedNewLap == 1 && cycArr[ii].isFinished == 0)
      tryAwardPoints_Cyc(&cycArr[ii]);
}

void tryAwardPlus20Points(Course *course) {
  Cyclist *first;
  Cyclist *second;

  first = course->firstPlaceCyc;
  second = course->secondPlaceCyc;
  
  if ((first->distTraveled - second->distTraveled) > ((1 + first->plus20Cnt)*((int)course->lenght))) {
    first->points += 20;
    first->plus20Cnt += 1;
  }
}

void updateCycs_Course(Course *course) {
  size_t ii;
  Cyclist *cyc;

  updateDist_Course(course);
  tryAwardPlus20Points(course);
  awardPoints_CycArr(course->cycArr, course->cycCnt);
  for (ii = 0, cyc = &course->cycArr[0]; ii < course->cycCnt; cyc = &course->cycArr[ii]) {
    if (cyc->isFinished == 0) {
      if (cyc->startedNewLap == 1) {
        updateSpeed_Cyc(cyc);
        tryFinish_Cyc(cyc);
        tryBreak_Cyc(cyc);
        cyc->startedNewLap = 0;
      }
    }
    ii++;
  }
}

