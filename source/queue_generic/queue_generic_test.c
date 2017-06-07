#include "defines.h"
#include "queue_generic.h"
#include "mu_test.h"

#include <stdlib.h>

typedef void(*ElementDestroy_t)(void* _item);

typedef struct Rect
{
	size_t m_height;
	size_t m_length;
} Rect_t;

void RectDestroy(Rect_t* _rect)
{
	free(_rect);
}

/*------------------------------------------------------------*/

UNIT(create_and_destroy)
	Queue* queue;
	queue = QueueCreate(10);
	QueueDestroy(queue, NULL);
	ASSERT_THAT(1 == 1);
END_UNIT

UNIT(double_destroy)
	ElementDestroy_t destroyFunc = (ElementDestroy_t) RectDestroy;
	int i;
	Rect_t* rect;
	Queue* queue;
	queue = QueueCreate(10);
	for(i = 0; i < 5; ++i)
	{
		rect = (Rect_t*) malloc(sizeof(Rect_t));
		QueueInsert(queue, rect);		
	}
	ASSERT_THAT(!QueueIsEmpty(queue));	
	QueueDestroy(queue, destroyFunc);
	QueueDestroy(queue, destroyFunc);
	ASSERT_THAT(!QueueIsEmpty(NULL));	
END_UNIT

UNIT(insert_and_remove)
	int i;
	Rect_t rects[10];
	Rect_t* rect;
	Queue* queue;
	queue = QueueCreate(100);
	ASSERT_THAT(QueueIsEmpty(queue));
	for(i = 0; i < 10; ++i)
	{
		rects[i].m_height = i;
		rects[i].m_length = i;
		QueueInsert(queue, &rects[i]);
	}
	ASSERT_THAT(!QueueIsEmpty(queue));
	for(i = 0; i < 10; ++i)
	{
		QueueRemove(queue, (void**)&rect);
		ASSERT_THAT(rect->m_height == i);
	}
	QueueDestroy(queue, NULL);
END_UNIT


TEST_SUITE(Generic Queue Tests)
	TEST(create_and_destroy)
	TEST(double_destroy)
	TEST(insert_and_remove)
END_SUITE

