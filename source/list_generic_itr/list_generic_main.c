#include "list.h"
#include "list_itr.h" 



/* USER input */


/*
//	This is implamintion of double linked list (node struct) Generic data type.
//	With itr pointer api
//	The main file call the functin and test them.
//
//	Yuval Hamberg Mar 2017
// 	yuval.hamberg@gmail.com
*/


void NodePrint(void* _node);
void NodeDataDestroy(void* _item);
bool PredicateIsNegativeNumber(void* _element, void* _context);
bool SortByAcend(void* _first, void* _second);

void Test_List_Create(void);
void Test_List_Destroy(void);
void Test_ListPushPopHead(void);
void Test_ListPushPopTail(void);
void Test_Count_Empty_Print(void);

void IsTestPassed(char _is_equal, int _expected, int _result);
int* CreateINT(int _value);
void MallocFail(void);



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

int main()
{
	
	#if !defined(NDEBUG) /* define for DEBUG */
	List_t* main_list;
	List_t* dest_list;
	List_Result result;
	Uint_t count;
	int data;
	void* holder;
	ListItr itr;
	ListItr itrNew;
	ListItr itr3;
	ListItr itr4;
	#endif
	
	/* MallocFail(); */ /* De-comment to activate malloc fail. NOTE! oprating takes a few minets. sucsses would be when all test fail but no crash */
	
	Test_List_Create();
	Test_List_Destroy();
	Test_ListPushPopHead();
	Test_ListPushPopTail();
	Test_Count_Empty_Print();
	
	
	
	#if !defined(NDEBUG) /* DEBUG */
	
	printf("\n\n\n--- Start Normal opration ---\n");
	
	main_list = List_Create();
	printf("\nList_Size empty test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	
	
	data = 7;
	result = List_PushHead(main_list, CreateINT(data += 2) );
	result = List_PushHead(main_list, CreateINT(data += 2) );
	result = List_PushHead(main_list, CreateINT(data += 2) );
	List_Print(main_list, NodePrint);
	
	result = List_PushTail(main_list, CreateINT(data += 2) );
	result = List_PushTail(main_list, CreateINT(9) );
	result = List_PushTail(main_list, CreateINT(data += 2) );
	List_Print(main_list, NodePrint);
	
	count = List_Size(main_list);
	printf("\nCount Test %u",count);
	IsTestPassed(TRUE, 6, List_Size(main_list));
	
	printf("\nIsListEmpty not empty test");
	IsTestPassed(FALSE, 0, List_Size(main_list));
	
	printf("\nList_PopHead return result test");
	result = List_PopHead(main_list, &holder);
	IsTestPassed(TRUE, OK, result);
	printf("\nList_PopHead return int value as expected");
	IsTestPassed(TRUE, 13, *(int*)holder);
	
	
	printf("\nList_PopTail return result test");
	result = List_PopTail(main_list, &holder);
	IsTestPassed(TRUE, OK, result);
	printf("\n List_PopTail return int value as expected");
	IsTestPassed(TRUE, 17, *(int*)holder);
	
	List_Print(main_list, NodePrint);
	
	itr = ListItr_End(main_list);
	printf("\n end: %p.", itr);
	holder = ListItr_Get(itr);
	if (NULL != holder) {
	printf(" in it %d value.", *(int*)holder );
	}
	
	itr = ListItr_Begin(main_list);
	printf("\n begin: %p.", itr);
	holder = ListItr_Get(itr);
	if (NULL != holder) {
	printf(" in it %d value.", *(int*)holder );
	}
	
	
	itr = ListItr_Next(itr);
	printf("\n next: %p.", itr);
	holder = ListItr_Get(itr);
	if (NULL != holder) {
	printf(" in it %d value.", *(int*)holder );
	}
	
	
	printf("\n Itr Equal test");
	IsTestPassed(TRUE, TRUE, ListItr_Equals(itr, itr ) );
	IsTestPassed(TRUE, FALSE, ListItr_Equals(itr, ListItr_Begin(main_list) ) );
	
	itr = ListItr_Prev(itr);
	printf("\n prev: %p.", itr);
	holder = ListItr_Get(itr);
	if (NULL != holder) {
	printf(" in it %d value.", *(int*)holder );
	}
	
	itr = ListItr_Prev(itr);
	printf("\n should not prev: %p.", itr);
	holder = ListItr_Get(itr);
	if (NULL != holder) {
	printf(" in it %d value.", *(int*)holder );
	}
	
	
	holder = ListItr_Set(itr, CreateINT(-5) );
	printf("\n Itr_SET got back %d value.", *(int*)holder );
	List_Print(main_list, NodePrint);
	
	itr = ListItr_End(main_list);
	itrNew = ListItr_InsertBefore(itr, CreateINT(-16) );
	printf("\n new item before itr: %p. itrNew: %p.", itr, itrNew);
	List_Print(main_list, NodePrint);
	
	printf("\nremove sntinal ");
	holder = ListItr_Remove(itr);
	if (NULL != holder) {
	printf(" after remove got %d value.", *(int*)holder );
	}
	
	printf("\nremove two before ");
	itr = ListItr_Prev(itr);
	itr = ListItr_Prev(itr);
	holder = ListItr_Remove(itr);
	if (NULL != holder) {
	printf(" after remove got %d value.", *(int*)holder );
	}
	List_Print(main_list, NodePrint);
	
	itr = ListItr_End(main_list);
	itr = ListItr_Prev(itr);
	
	itr = ListItr_FindFirst(ListItr_Begin(main_list), itr, PredicateIsNegativeNumber, NULL);
	holder = ListItr_Get(itr);
	printf("\n ListItr_FindFirst got back %d value from itr:%p.", *(int*)holder , itr);
	IsTestPassed(TRUE, -5, *(int*)holder );
	
	itr = ListItr_End(main_list);
	count = ListItr_CountIf(ListItr_Begin(main_list), itr, PredicateIsNegativeNumber, NULL);
	printf("\n ListItr_CountIf returnd %u." , count );
	IsTestPassed(TRUE, 2, count );
	
	itr = ListItr_End(main_list);
	itr = ListItr_Prev(itr);
	
	itr = ListItr_ForEach(ListItr_Begin(main_list), itr, PredicateIsNegativeNumber, NULL);
	holder = ListItr_Get(itr);
	printf("\n ListItr_ForEach got back %d value from itr:%p.", *(int*)holder , itr);
	IsTestPassed(TRUE, 9, *(int*)holder );
	
	printf("\n\n ListItr_Sort:\n ");
	itr = ListItr_End(main_list);
	ListItr_Sort(ListItr_Begin(main_list), itr, SortByAcend);
	List_Print(main_list, NodePrint);
	
	printf("\n\n ListItr_Splice:\n ");
	itr = ListItr_Begin(main_list);
	itr = ListItr_Next(itr);
	ListItr_Splice(ListItr_Begin(main_list), itr, ListItr_Next(itr));
	List_Print(main_list, NodePrint);
	
	List_PushTail(main_list, CreateINT(9) );
	List_PushTail(main_list, CreateINT(-16) );
	List_PushTail(main_list, CreateINT(88) );
	List_Print(main_list, NodePrint);
	
	printf("\n\n ListItr_Unique:\n ");
	dest_list = ListItr_Unique(ListItr_Begin(main_list), ListItr_End(main_list), IsEqualInt);
	List_Print(main_list, NodePrint);
	List_Print(dest_list, NodePrint);
	List_Destroy(&dest_list, NodeDataDestroy);
	
	
	printf("\n\n ListItr_Merge:\n ");
	List_PushTail(main_list, CreateINT(52) );
	List_PushTail(main_list, CreateINT(23) );
	List_PushTail(main_list, CreateINT(-5) );
	List_Print(main_list, NodePrint);
	
	itr = ListItr_Begin(main_list);
	itr = ListItr_Next(itr);
	itrNew = ListItr_Next(itr);
	itrNew = ListItr_Next(itrNew);
	itr3 = ListItr_Next(itrNew);
	itr3 = ListItr_Next(itr3);
	itr4 = ListItr_Next(itr3);
	itr4 = ListItr_Next(itr4);
	
	/* 0, 1, 3, 5, 7*/
	ListItr_Merge(ListItr_Begin(main_list), itr, itrNew, itr3, itr4, SortByAcend);
	List_Print(main_list, NodePrint);
	
	
	
	printf("\n\n ListItr_Cut:\n ");
	itr = ListItr_Begin(main_list);
	itr = ListItr_Next(itr);
	dest_list = ListItr_Cut(itr, ListItr_Next(itr));
	List_Print(main_list, NodePrint);
	List_Print(dest_list, NodePrint);
	
	
	
	List_Destroy(&main_list, NodeDataDestroy);
	List_Destroy(&main_list, NodeDataDestroy); /* duble free test */
	
	#endif
	
	printf("\n--- END ---\n");
	return OK;
}

void Test_List_Create(void)
{
	List_t* main_list;

	printf("\n\nTest_List_Create\n---------------");
	
	main_list = List_Create();
	
	printf("\nIsListEmpty test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	
	printf("\nCount Test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	
	List_Destroy(&main_list, NULL);
	return;
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



/* Testing function */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void IsTestPassed(char _is_equal, int _expected, int _result)
{
	if (_is_equal == TRUE)
	{
		if (_expected == _result)
		{
			printf(GRN "\tPASS" RESET);
		}
		else
		{
			
			printf(RED "\tFAILD! FAILD! FAILD!" RESET);
		}
	}
	if (_is_equal == FALSE)
	{
		if (_expected != _result)
		{
			printf(GRN "\tPASS" RESET);
		}
		else
		{
			printf(RED "\tFAILD! FAILD! FAILD!" RESET);
		}
	}	
	
}


int* CreateINT(int _value)
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


void Test_List_Destroy(void)
{
	/* void List_Destroy(List_t* _list) */ 
	List_t* main_list = 0;
	
	printf("\n\nTest_List_Destroy\n---------------");
	List_Destroy((void*) main_list, NodeDataDestroy);
	printf("\nPass not crushing after empty input");
	
	/* NOT WORKING. main doen't have accses to node. why? 
	main_list = List_Create();
	main_list->m_tail.m_data = 0xdeadbeef;
	List_Destroy(main_list, NULL);
	printf("\nPass not crushing after MAGICNUMBER DEAD input");
	*/
	
	printf("\nNormal destroy Test");
	main_list = List_Create();
	List_Destroy((void*) main_list, NULL);
	printf(" Pass not crushing");
	printf("\nDouble destroy Test");
	List_Destroy((void*) main_list, NULL);
	printf(" Pass not crushing");
	
	return;
}



void Test_ListPushPopHead(void)
{
	/* List_Result List_PushHead(List_t* _list, int _data); */
	List_t* main_list;
	List_Result result;
	void* data;
	
	main_list = List_Create();
	printf("\n\nTest_ListPushPopHead\n---------------");
	
	result = List_PushHead(main_list, CreateINT(11) );
	printf("\nreturn result test");
	IsTestPassed(TRUE, OK, result);
	printf("\nIsListEmpty test");
	IsTestPassed(FALSE, 0, List_Size(main_list));
	printf("\n++Count Test");
	IsTestPassed(TRUE, 1, List_Size(main_list));
	
	result = List_PushHead(main_list, CreateINT(22) );
	printf("\n++Count Test");
	IsTestPassed(TRUE, 2, List_Size(main_list));
	
	
	printf("\nNULL Error Test");
	result = List_PushHead(NULL, CreateINT(3) );
	IsTestPassed(TRUE, LIST_UNITIALIZED_ERROR, result);
	printf("\nNULL Count Test");
	IsTestPassed(TRUE, 2, List_Size(main_list));
	
	result = List_PopHead( (void*) main_list, &data);
	printf("\nreturn normal result test");
	IsTestPassed(TRUE, OK, result);
	printf("\nNULL Count Test");
	IsTestPassed(TRUE, 1, List_Size(main_list));
	printf("\nreturn int value as expected");
	IsTestPassed(TRUE, 22, *(int*) data); 
	
	printf("\nNULL list Error Test");
	result = List_PopHead(NULL, &data);
	IsTestPassed(TRUE, LIST_UNITIALIZED_ERROR, result);
	
	printf("\nNULL data Error Test");
	result = List_PopHead(main_list, NULL);
	IsTestPassed(TRUE, LIST_UNITIALIZED_ERROR, result);
	
	result = List_PopHead(main_list, &data);
	printf("\nreturn normal result test (last item in list)");
	IsTestPassed(TRUE, OK, result);
	printf("\nNULL Count Test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	printf("\nreturn int value as expected");
	IsTestPassed(TRUE, 11, *(int*) data);
	printf("\nIsListEmpty test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	
	result = List_PopHead(main_list, &data);
	printf("\npop from empty list");
	IsTestPassed(TRUE, LIST_UNDERFLOW, result);
	IsTestPassed(TRUE, 0, List_Size(main_list));
	printf("\nIsListEmpty test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	
	List_Destroy(&main_list, NodeDataDestroy);
	
	return;
}



void Test_ListPushPopTail(void)
{
	/* List_Result List_PushTail(List_t* _list, int _data); */

	List_t* main_list;
	List_Result result;
	void* data;
	
	main_list = List_Create();
	printf("\n\nTest_ListPushPopTail\n---------------");
	
	result = List_PushTail(main_list, CreateINT(33));
	printf("\nreturn result test");
	IsTestPassed(TRUE, OK, result);
	printf("\nIsListEmpty test");
	IsTestPassed(FALSE, 0, List_Size(main_list));
	printf("\n++Count Test");
	IsTestPassed(TRUE, 1, List_Size(main_list));
	
	result = List_PushTail(main_list, CreateINT(44));
	printf("\n++Count Test");
	IsTestPassed(TRUE, 2, List_Size(main_list));
	
	
	printf("\nNULL Error Test");
	result = List_PushTail(NULL, CreateINT(3));
	IsTestPassed(TRUE, LIST_UNITIALIZED_ERROR, result);
	printf("\nNULL Count Test");
	IsTestPassed(TRUE, 2, List_Size(main_list));
	
	result = List_PopTail(main_list, &data);
	printf("\nreturn normal result test");
	IsTestPassed(TRUE, OK, result);
	printf("\nNULL Count Test");
	IsTestPassed(TRUE, 1, List_Size(main_list));
	printf("\nreturn int value as expected");
	IsTestPassed(TRUE, 44, *(int*)data);
	
	printf("\nNULL list Error Test");
	result = List_PopTail(NULL, &data);
	IsTestPassed(TRUE, LIST_UNITIALIZED_ERROR, result);
	
	printf("\nNULL data Error Test");
	result = List_PopTail(main_list, NULL);
	IsTestPassed(TRUE, LIST_UNITIALIZED_ERROR, result);
	
	result = List_PopTail(main_list, &data);
	printf("\nreturn normal result test (last item in list)");
	IsTestPassed(TRUE, OK, result);
	printf("\nNULL Count Test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	printf("\nreturn int value as expected");
	IsTestPassed(TRUE, 33, *(int*)data);
	printf("\nIsListEmpty test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	
	result = List_PopTail(main_list, &data);
	printf("\npop from empty list");
	IsTestPassed(TRUE, LIST_UNDERFLOW, result);
	IsTestPassed(TRUE, 0, List_Size(main_list));
	printf("\nIsListEmpty test");
	IsTestPassed(TRUE, 0, List_Size(main_list));
	
	List_Destroy(&main_list, NodeDataDestroy);
	
	return;	
}

void Test_Count_Empty_Print(void)
{
	size_t size;
	
	printf("\n\nTest_Count_Empty_Print\n---------------");

	/* size_t List_Size(List_t* _list); */
	size = List_Size(NULL);
	printf("\nPass List_Size not crushing after empty input");
	printf("\nList_Size return when null");
	IsTestPassed(TRUE, 0, size);
	
	
	/* int IsListEmpty(List_t* _list); */
/*	integer = IsListEmpty(NULL);
	printf("\nPass IsListEmpty not crushing after empty input");
	printf("\nIsListEmpty return when null");
	IsTestPassed(TRUE, TRUE, integer);
*/	
	
	
	/* void List_Print(List_t* _list); */
	List_Print(NULL, NULL);
	printf("\nPass List_Print not crushing after empty input");
	
	
	return;
}
