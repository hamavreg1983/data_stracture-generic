#include <stdio.h>
#include <stdlib.h>
#include "mu_test.h"
#include "stack_generic.h"



/*	This libary builds and oprate data stratue of stack type, genric data type.
// 	on top of list_generic.a libray
//
//	Test file
//	
//	Yuval Hamberg March 2017
//	yuval.hamberg@gmail.com
 */
 
 
 
void NodePrint(void* _node);
void NodeDataDestroy(void* _item);
bool PredicateIsNegativeNumber(void* _element, void* _context);
bool SortByAcend(void* _first, void* _second);

void* CreateINT(int _value);
void MallocFail(void);


UNIT(Create)
	Stack* mainStack = NULL;

	mainStack = StackCreate();
	
	ASSERT_THAT( NULL != mainStack);
	ASSERT_THAT( 0 == List_Size(mainStack));
	
	StackDestroy(&mainStack, NodeDataDestroy);
END_UNIT

UNIT(CreateMallocFail)
	Stack* mainStack = NULL;
	
	MallocFail();
	
	mainStack = StackCreate();
	
	ASSERT_THAT( NULL == mainStack);
	ASSERT_THAT( 0 == List_Size(mainStack));
	
	StackDestroy(&mainStack, NodeDataDestroy);
END_UNIT



UNIT(Destory)
	Stack* mainStack = NULL;
	mainStack = StackCreate();
	
	ASSERT_THAT( NULL != mainStack);
	StackDestroy(&mainStack, NodeDataDestroy);
	ASSERT_THAT( NULL == mainStack);
	
	
	
END_UNIT

UNIT(DestoryNULL)
	Stack* mainStack;
	int test = 1;
	Stack* testPtr = (Stack*)&test;
	
	mainStack = StackCreate();
	StackDestroy(&mainStack, NULL);
	ASSERT_THAT( NULL == mainStack);
	
	mainStack = StackCreate();
	StackDestroy(&testPtr, NULL);
	ASSERT_THAT( NULL != testPtr);
	
	
END_UNIT

UNIT(Push)
	Stack* mainStack;
	List_Result result;
	void* in;
	int num = 7;
	
	in = CreateINT(num);
	
	mainStack = StackCreate();
	
	result = StackPush(mainStack, in);
	ASSERT_THAT( FALSE == StackIsEmpty(mainStack) );
	ASSERT_THAT( LIST_SUCCESS == result );
	
	
	StackDestroy(&mainStack, NodeDataDestroy);
END_UNIT



UNIT(Pop)
	Stack* mainStack;
	List_Result result;
	void* in;
	void* out;
	int num = 7;
	
	in = CreateINT(num);
	
	mainStack = StackCreate();
	
	StackPush(mainStack, in);
	
	result = StackPop(mainStack, &out);
	
	ASSERT_THAT( num == *(int*)out);
	ASSERT_THAT( LIST_SUCCESS == result );
	
	result = StackPop(mainStack, &out);
	ASSERT_THAT( LIST_UNDERFLOW == result );
	
	StackDestroy(&mainStack, NodeDataDestroy);
END_UNIT


UNIT(Top)
	Stack* mainStack;
	List_Result result;
	void* in;
	void* out;
	int num = 7;
	
	in = CreateINT(num);
	
	mainStack = StackCreate();
	
	StackPush(mainStack, in);
	
	result = StackTop(mainStack, &out);
	
	ASSERT_THAT( num == *(int*)out);
	ASSERT_THAT( LIST_SUCCESS == result );
	
	result = StackTop(mainStack, &out);
	ASSERT_THAT( num == *(int*)out);
	ASSERT_THAT( LIST_SUCCESS == result );
	
	StackDestroy(&mainStack, NodeDataDestroy);
END_UNIT



UNIT(IsEmpty)
	Stack* mainStack;
	mainStack = StackCreate();
	
	ASSERT_THAT( TRUE == StackIsEmpty(mainStack) );
	
	StackPush(mainStack, CreateINT(5));
	ASSERT_THAT( FALSE == StackIsEmpty(mainStack) );
	
	StackPush(mainStack, CreateINT(55));
	ASSERT_THAT( FALSE == StackIsEmpty(mainStack) );
	
	StackDestroy(&mainStack, NodeDataDestroy);
END_UNIT




UNIT(malloc_zero_will_alloc_something)
	int *p = malloc(0);
	ASSERT_THAT(p != NULL);
END_UNIT




TEST_SUITE(framework test)
	TEST(Create)
/*	TEST(CreateMallocFail)*/
	TEST(Destory)
	TEST(DestoryNULL)
	TEST(Push)
	TEST(Pop)
	TEST(Top)
	TEST(IsEmpty)
	
	TEST(malloc_zero_will_alloc_something)
END_SUITE






void NodePrint(void* _item)
{
	if (NULL == _item)
	{
		return;
	}
	
	printf(" %d",*(int *) _item);
	
	return;
}

void NodeDataDestroy(void* _item)
{
	if (NULL == _item)
	{
		return;
	}
	
	free(_item);
	
	return;
}

bool PredicateIsNegativeNumber(void* _element, void* _context)
{
	if (*(int*)_element < 0)
	{
		return TRUE;
	}
	return FALSE;
}

bool SortByAcend(void* _first, void* _second)
{
	if (*(int*)_first > *(int*)_second)
	{
		return TRUE;
	}
	return FALSE;
}

bool IsEqualInt(void* _a, void* _b)
{
	if (*(int*)_a == *(int*)_b)
	{
		return TRUE;
	}
	return FALSE;
}

void* CreateINT(int _value)
{
	int* item;
	
	item = (int *) malloc(1 * sizeof(int)); 
	if (NULL == item)
	{
		return NULL;
	}
	
	*item = _value;
	
	return item;
}

void MallocFail(void) /* stress test of malloc */
{
	void* test;
	Uint_t malloc_counter = 0;
	
	printf("\nNULL Pointer TEST is on.\n--------------------------");
	
	while(1)
	{
		test = malloc(sizeof(double)) ;
		++malloc_counter;
		if (NULL == test)
		{
			printf("\nmalloc failed at size %u.",malloc_counter * sizeof(double));
			break;
		}
	}
}
