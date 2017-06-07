#ifndef __PERIODIC_EXECUTOR_DB__
#define __PERIODIC_EXECUTOR_DB__

#include <stddef.h>  /* for size_t */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* printf for print function and debug mode  */

#include "defines.h"
#include "periodicExecutor.h"

#define INITIAL_CAPACITY 10
#define BLOCK_SIZE 10

/*
//	This is PERIODIC_EXECUTOR user front file.
// 	This moudle does with selected data Stracture.
//
//	Yuval Hamberg April 2017
// 	yuval.hamberg@gmail.com
*/

/* This moudle surruneds DB data strature. so the api would be simmiler to heap and vector */
/* this moudle would first creat the vecter that would be sent to heap without periodicExecutor need to deal with it */

typedef enum DB_Result {
	DB_SUCCESS = 0,
	DB_NOT_INITIALIZED,
	DB_REALLOCATION_FAILED
} DB_Result;

typedef struct DB DB;
typedef DB DB_t;

/*
typedef int	(*ActionFunction)(const void *_elem, void * _context);
*/




/* DB* DB_Build(Vector* _vector, LessThanComparator _pfLess);
// spare the user the vector create call
*/
DB* DB_Build(LessThanComparator _pfLess); 

/**  
 * @brief Deallocate a previously allocated heap
 * Frees the heap but not the underlying vector  
 * @param[in] _db - DB to be deallocated.  On success will be nulled.
 * @return Underlying vector
 */
/* Vector* DB_Destroy(DB** _db);
// spare the user the vector destory call */
void DB_Destroy(DB** _db, void (*_elementDestroy)(void* _item)); /* TODO user function */

/**  
 * @brief Add an element to the DB preserving heap property.  
 * @param[in] _db - DB to insert element to.
 * @param[in] _element - Element to insert.
 * @return success or error code 
 * @retval HEAP_OK  on success
 * @retval HEAP_NOT_INITIALIZED  error, heap not initilized
 * @retval HEAP_REALLOCATION_FAILED error, heap could not reallocate underlying vector 
 */
DB_Result DB_Insert(DB* _db, void* _element);

/**  
 * @brief Peek at element on top of heap without extracting it.  
 * @param[in] _db - DB to peek to.
 * @return pointer to element, can be null if heap is empty. 
 */
const void* DB_Peek(const DB* _db); 

/**  
 * @brief Extract element on top of heap and remove it.  
 * @param[in] _db - DB to extract from.
 * @return pointer to element, can be null if heap is empty. 
 */
void* DB_Extract(DB* _db);

/**  
 * @brief Get the current size (number of elements) in the heap.  
 * @param[in] _db - DB to extract from.
 * @return number of elements or zero if empty. 
 */
size_t DB_Size(const DB* _db);

/**  
 * @brief Iterate over all elements  in the heap from top to bottom.
 * @details The user provided ActionFunction _act will be called for each element.  
 * @param[in] _db - DB to iterate over.
 * @param[in] _act - User provided function pointer to be onvoked for each element
 * @returns number of times the user functions was invoked
 */
/* size_t DB_ForEach(const DB* _db, ActionFunction _act, void* _context); */



void DB_Print(DB_t* _db, SimpleJobFunc _printItem);

void DB_Print_Graphic(DB_t* _db, SimpleJobFunc _printItem);


#endif /* __PERIODIC_EXECUTOR_DB__ */
