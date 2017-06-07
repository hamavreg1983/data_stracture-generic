#ifndef __GEN_QUEUE_H__
#define __GEN_QUEUE_H__

/** 
 *  @file gen_queue.h
 *  @brief create a que of void pointer. 
 * 
 *  @details FIFO system. An implamantion of circul queue for generic data.
 * 
 *  @author Author Yuval Hamberg (yuval.hamberg@gmail.com) 
 *	@date April 2017
 * 
 *  @bug No known bugs. 
 */ 

#include <stddef.h> /* size_t */
#include "defines.h"

/* ~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct Queue Queue;

typedef enum
{
	QUEUE_SUCCESS = 0,
	QUEUE_UNITIALIZED_ERROR,
	QUEUE_ALLOCATION_ERROR,	
	QUEUE_INDEX_OUT_OF_BOUNDS_ERROR,
	QUEUE_OVERFLOW,
	QUEUE_UNDERFLOW,
	QUEUE_REALLOCATION_FAILED,
	QUEUE_WRONG_INDEX,
	QUEUE_NODE_UNITIALIZED_ERROR,
	QUEUE_DATA_NULL

} Queue_Result;


/* ~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/* ~~~ API function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/** @brief QueueCreate
 *  @details Creates queue.
 *
 * @params[in] _size _msgQue: initial size of queue. Size must be positive number. this number is unchangble.
 *
 * @returns returns pointer to the created queue.
 * @returns Returns NULL on error
 * @retval Queue*
 */
Queue* QueueCreate(size_t _size);



/** @brief QueueDestroy
 *  @details Destroys queue
 *
 * @params[in] _queue: Recieves pointer to the queue to be destroyed
 * @params[in] _destroyElement: function that destory void* items
 *
 * @retval void
 */
void QueueDestroy(Queue* _queue, void(*_destroyElement)(void*));



/** @brief QueueInsert
 *  @details Inserts value in queue
 *
 * @params[in] _queue: Recieves queue pointer
 * @params[in] _item: value to be inserted
 *
 * @returns Returns error codes
 * @retval Queue_Result
 */
Queue_Result QueueInsert(Queue* _queue, void* _item);




/** @brief QueueRemove
 *  @details Removes value from the queue and stores it in variable
 *
 * @params[in] _queue: queue pointer
 *
 * @params[out] _item: pointer to the variable in which the retrieved value to be stored
 *
 * @returns Returns error codes
 * @retval Queue_Result
 */
Queue_Result QueueRemove(Queue* _queue, void** _item);



/** @brief QueueIsEmpty
 *  @details 
 *
 * @params[in] _queue: Recieves pointer to the queue
 *
 * @returns  TRUE if the queue is empty
 * @returns FALSE if not empty. or FALSE if failed
 * @retval int
 */
int QueueIsEmpty(Queue* _queue);



/** @brief Queue_Count
 *  @details Returns number of item in queue
 *
 * @params[in] _queue: Recieves pointer to the queue
 *
 * @returns Returns number of item in queue
 * @returns -1 if failed
 * @retval int
 */
int Queue_Count(Queue* _queue);



/* debug */
void QueuePrint(Queue* _queue, void (*_elementPrint)(void* _item));


#endif /*#ifndef __QUEUE_H__*/
