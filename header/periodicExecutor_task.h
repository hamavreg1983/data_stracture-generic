
#ifndef __PERIODIC_EXECUTOR_TASK__
#define __PERIODIC_EXECUTOR_TASK__

#include <stddef.h>  /* for size_t */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* printf for print function and debug mode  */

#include "defines.h"
#include "periodicExecutor.h"
#include "periodicExecutor_clock.h"
#include "periodicExecutor_db.h"


/*
//	This is PERIODIC_EXECUTOR user front file.
// 	The modle deals with indavidual tasks.
//
//	Yuval Hamberg April 2017
// 	yuval.hamberg@gmail.com
*/

typedef struct Taskie_t Taskie_t;


/*
// Create task 
// recives: _executor is the PeriodicExecutor. _task is represented by a user provided function pointer so is _context. _period_ms is the (freq) period of recurrence in milli seconds.
// returns pointer to new tast (taskie*) to be placed in data stracture.
// if failed return NULL;
*/
Taskie_t* CreateTask(DB_t* _db, clockid_t _clk_id, Task _func, void* _context, size_t _period_ms); 

/* 	destory and free a task 
	recives a pointer to task
	return nothing no matter what happens
*/
void DestroyTask(Taskie_t* _task);

/* when you have a complete task, add it to DataBase */
void PushTask(DB_t* _db, Taskie_t* _task);


/* 	function run the user function with its context.
	return status from function. 0 means re run is needed, anyother number means the functoin is done.
*/
int RunFunc(Taskie_t* _task);



/* 	function sets the new time a task should run.
	calls clock to get time and update the task.
*/
void UpdateWhenToRunTask(Taskie_t* _task);

bool IsFirstTaskLaterThanSecondTask(Taskie_t* _aTask, Taskie_t* _bTask);

void SleepUntilTaskTime(Taskie_t* _task);

void PrintTask(Taskie_t* _task);


#endif /* __PERIODIC_EXECUTOR_TASK__ */
