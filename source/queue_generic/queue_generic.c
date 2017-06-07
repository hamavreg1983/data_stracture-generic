#include <stdlib.h>

#include "defines.h"
#include "queue_generic.h"
#include "list.h"


/* ~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define ALIVE_MAGIC_NUMBER	0xfadeface
#define DEAD_MAGIC_NUMBER	0xdeadface


/* ~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

struct Queue
{
	size_t m_magicNumber;
	List* m_list;
	Uint m_numItems;
	Uint m_maxNumItems;
};

/* ~~~ Internal function forward declartion ~~~~~~~~~~~~~~~~~~~~~~~~ */

static Queue_Result ListErrToQueueErr(List_Result _err);


/* ~~~ API function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

Queue* QueueCreate(size_t _size)
{
	Queue* queue;
	
	queue = (Queue*) malloc(sizeof(Queue));
	
	RETURN_IF_EQUAL(queue, NULL ,NULL)
	
	queue->m_list = List_Create();
	if(NULL == queue->m_list)
	{
		free(queue);
		return NULL;
	}
	
	queue->m_numItems = 0;
	queue->m_maxNumItems = _size;
	
	return queue;
}

void QueueDestroy(Queue* _queue, void(*_destroyElement)(void*))
{
	if(NULL != _queue && ALIVE_MAGIC_NUMBER == _queue->m_magicNumber)
	{
		List_Destroy(&(_queue->m_list), _destroyElement);
		_queue->m_magicNumber = DEAD_MAGIC_NUMBER;
		free(_queue);
	}
	
	return;
}

Queue_Result QueueInsert(Queue* _queue, void* _item)
{
	List_Result lRes;
	if (_queue->m_numItems >= _queue->m_maxNumItems)
	{
		lRes = LIST_OVERFLOW;
	}
	else
	{
		lRes = List_PushHead(_queue->m_list, _item);
		if (LIST_SUCCESS == lRes)
		{
			++_queue->m_numItems;
		}
	}
	return ListErrToQueueErr(lRes);
}


Queue_Result QueueRemove(Queue* _queue, void** _item)
{
	List_Result lRes;
	if (QueueIsEmpty(_queue))
	{
		lRes = LIST_UNDERFLOW;
	}
	else
	{
		lRes = List_PopTail(_queue->m_list, _item);
		if (LIST_SUCCESS == lRes)
		{
			--_queue->m_numItems;
		}
	}
	return ListErrToQueueErr(lRes);
}


bool QueueIsEmpty(Queue* _queue)
{
	RETURN_IF_EQUAL(_queue, NULL , FALSE)
	return (List_Size(_queue->m_list) == 0 ? TRUE : FALSE);
}


int Queue_Count(Queue* _queue)
{
	RETURN_IF_EQUAL(_queue, NULL , -1)
	return (List_Size(_queue->m_list));
}


void QueuePrint(Queue* _queue, void (*_elementPrint)(void* _item))
{
	List_Print(_queue->m_list, _elementPrint);
	return;
}

/* ~~~ Internal function  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static Queue_Result ListErrToQueueErr(List_Result _err)
{
	Queue_Result queueRes;
	switch(_err)
	{
		case LIST_SUCCESS:
			queueRes = QUEUE_SUCCESS;
			break;
		case LIST_UNITIALIZED_ERROR:
			queueRes = QUEUE_UNITIALIZED_ERROR;
			break;
		case LIST_ALLOCATION_ERROR:
			queueRes = QUEUE_ALLOCATION_ERROR;
			break;
		case LIST_INDEX_OUT_OF_BOUNDS_ERROR:
			queueRes = QUEUE_INDEX_OUT_OF_BOUNDS_ERROR;
			break;
		case LIST_OVERFLOW:
			queueRes = QUEUE_INDEX_OUT_OF_BOUNDS_ERROR;
			break;
		case LIST_UNDERFLOW:
			queueRes = QUEUE_UNDERFLOW;
			break;
		case LIST_REALLOCATION_FAILED:
			queueRes = QUEUE_REALLOCATION_FAILED;
			break;
		case LIST_WRONG_INDEX:
			queueRes = QUEUE_WRONG_INDEX;
			break;
		case LIST_NODE_UNITIALIZED_ERROR:
			queueRes = QUEUE_NODE_UNITIALIZED_ERROR;
			break;
		case LIST_DATA_NULL:
			queueRes = QUEUE_DATA_NULL;
			break;
	}
	return queueRes;
}











