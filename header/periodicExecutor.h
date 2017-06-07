#ifndef __PERIODIC_EXECUTOR__
#define __PERIODIC_EXECUTOR__

#include <stddef.h>  /* for size_t */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* printf for print function and debug mode  */



#include "heap_tree_generic.h" /* SimpleJobFunc */
#include "defines.h"

#define NDEBUG
/* remove to turn on debug printout */
/* #undef NDEBUG */
 
/*
//	This is PERIODIC_EXECUTOR user front file.
// 	The main mangar of the program.
//
//	Yuval Hamberg April 2017
// 	yuval.hamberg@gmail.com
*/


#define PE_NAME_LENGTH 32
#define NO_RE_RUN 0

typedef struct PeriodicExecutor PeriodicExecutor;
typedef PeriodicExecutor PeriodicExecutor_t;

typedef int(*Task)(void* _context);

/* typedef bool	(*LessThanComparator)(const void *_left, const void *_right); Defined in heap_tree_generic.h */ 

/* typedef void (*SimpleJobFunc)(void* _item); Defined in heap_tree_generic.h*/ 


/* typedef enum PE_Result {
//	PE_SUCCESS = 0,
//	PE_UNITIALIZED_ERROR,				< Uninitialized PE error 	
//	PE_ALLOCATION_ERROR,				< realloc error on grow/shrink   
//	PE_INDEX_OUT_OF_BOUNDS_ERROR,
//	PE_OVERFLOW,
//	PE_UNDERFLOW,
//	PE_REALLOCATION_FAILED,
//	PE_WRONG_INDEX,
//	PE_UNKNOWN_ERROR
//} PE_Result;
*/

/*
// Create a periodic executor that will measure time using given clock. 
// Clockid_t specify the id of the system clock to be used. CLOCK_REALTIME, 
// CLOCK_REALTIME_COARSE, CLOCK_MONOTONIC and similar provided its 
// supported by the underlying system. 
*/
PeriodicExecutor* PeriodicExecutor_Create(const char* _name, clockid_t _clk_id );

/* Destroy previously created executor. Cleaning all allocated memory and resources.  */
void PeriodicExecutor_Destroy(PeriodicExecutor* _executor);


/*
// Adding task to the executor 
// recives: _executor is the PeriodicExecutor. _task is represented by a user provided function pointer so is _context. _period_ms is the (freq) period of recurrence in milli seconds.
// returns 1 TRUE is sucssful or 0 FALSE if failed
*/
int PeriodicExecutor_Add(PeriodicExecutor* _executor, Task _func, void* _context, size_t _period_ms); 
 

/* Start running the tasks previously added to the executor or resume a previously paused  
// Executor. This function will return in one of two cases: 
// 1. The tasks were executed to completion. 
// 2. The executor was paused 
// This function returns the number of tasks run cycle.  */
size_t PeriodicExecutor_Run(PeriodicExecutor* _executor); 
 

/* Pause running the executor 
// Pause the execution of the executor. If a task is currently being executed then pause after 
// it has been completed current cycle. 
// This function returns the number of tasks remaining inside the executor. 
// If failed, returns zero. */
size_t PeriodicExecutor_Pause(PeriodicExecutor* _executor); 
 






#endif /* __PERIODIC_EXECUTOR__ */
