#ifndef __PERIODIC_EXECUTOR_CLOCK__
#define __PERIODIC_EXECUTOR_CLOCK__

#include <stdio.h> /* printf for print function and debug mode  */
#include <stdlib.h> /* malloc */

#include <time.h> /* clock functions */
#include <unistd.h> /* usleep */
#include <sys/types.h> /* clockid_t */

#include "periodicExecutor.h"
#include "defines.h"

/*
//	This is PERIODIC_EXECUTOR user front file.
// 	The moudle does the clock work.
//
//	Yuval Hamberg April 2017
// 	yuval.hamberg@gmail.com
*/

/* Time calculation should be based on timespec data structure and using these functions: 
// usleep 
// clock_gettime
*/

typedef struct timespec timespec_t;

typedef struct Clock Clock_t;


/*	This function get the current time in miliseconds sence the unix epoc
	it recives the time methode: CLOCK_REALTIME,  CLOCK_REALTIME_COARSE, CLOCK_MONOTONIC
	returns clock with the time to run.
	NULL if failed.
 */
Clock_t* CreateCurrentTime(clockid_t _clk_id);

/*	destroy and free Clock_t*
	return nothing no matter what */ 
void DestroyCurrentTime(Clock_t* _clock);


void SleepUntilNextJob(Clock_t* _clock);


/* 	Function get the freq of a task in miliseconds
	returns clock with the time to run.
	NULL if failed.
	*/
void CalculateDelay(Clock_t* _clock, size_t _freqToRun_ms);


/* compare two clocks. if a is sooner than b return TRUE, if eqeual down to microseconds return FALSE. if b is sooner than FALSE */
bool IsFirstTimeLaterThanSecondTime(Clock_t* _a, Clock_t* _b);



/* get clock_t and return the time in a microsecnd long unsigned int */
LUint ConvertToMicroSec(Clock_t* _clock);



#endif /* __PERIODIC_EXECUTOR_CLOCK__ */
