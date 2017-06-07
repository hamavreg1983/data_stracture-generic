#include "list_itr.h"
#include "list.h"
#include "list_functions.h"
#include <stddef.h>  /* for size_t */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* printf for print function and debug mode  */

#define ALIVE_MAGIC_NUMBER 0xbeefbeef
#define DEAD_MAGIC_NUMBER 0xdeadbeef

/*
//	This is implamintion of double linked list (node struct) Generic data type.
//	With itr pointer api
//
//	Yuval Hamberg Mar 2017
// 	yuval.hamberg@gmail.com
*/

typedef struct Node Node_t;

struct Node
{
    void* m_data;
    struct Node* m_next;
    struct Node* m_prev;
    int m_magicNumber;
};

struct List
{
    struct Node m_head;
    struct Node m_tail;
    int m_magicNumber;
};

/* forward deaclartion */
/* *********************/

/* just calling the free command on a node and setting the maic number to dead */
static void FreeNode(Node_t* _node); 

/* inactive old function */
/* static List_Result DestroyNode(Node_t* _node, void* _data); */

/* main engine for the push actions. recives (_item) the new data to be insearted. The new item would be insearted to a new node and incorparted to the list at location one after node pointer. */
static List_Result List_PushNode(Node_t* _node, void* _item);

/* main engine for the pop actions. recives (_item) where the new data to be returned. the node would be removed and destoried. the user responsibul for freeing the item */
static List_Result List_PopNode(Node_t* _node, void** _pItem);

/* set the m_next and m_prev links. _node is the new node to be insearted and _before_node marks the spot after it the new node would be incurpareted in the list. this function workd on the _node, _before_node AND the node after _node. */
static void NodeInsertLinkerFix(Node_t* _node, Node_t* _before_node);

/* set the m_next and m_prev links. _node is the node to be removed. this function workd on the _node, node before AND the node after. */
static 

void NodeRemoveLinkerFix(Node_t* _node);

/* TODO comments for all internal functions */
static bool CheckNodeParam(const Node_t* _node);


static bool CheckListParam(const List_t* _list);

static bool swapPtr(void** first, void** second);

static void MoveNodes (ListItr _dest, ListItr _begin, ListItr _end);

static Node_t* MoveData (ListItr _dest, ListItr _source);

/* Functions */
/* *********************/


List* List_Create(void)
{
	List_t* new_list;
	
	new_list = (List_t*) malloc(sizeof(List_t));
	if (NULL == new_list) /* malloc fail */
	{
		return NULL;
	}
		
	new_list->m_magicNumber = ALIVE_MAGIC_NUMBER;

	new_list->m_head.m_next = &(new_list->m_tail);
	new_list->m_head.m_prev = NULL;
	new_list->m_tail.m_next = NULL;
	new_list->m_tail.m_prev = &(new_list->m_head);
	
	new_list->m_head.m_magicNumber = ALIVE_MAGIC_NUMBER;
	new_list->m_tail.m_magicNumber = ALIVE_MAGIC_NUMBER;
	
 /* TODO sentinals node data should be set NULL */

	return new_list;
}

void List_Destroy(List** _list, void (*_elementDestroy)(void* _item))
{
	Node_t* ptr = 0;
	
	if ( NULL == _list || !CheckListParam(*_list) )
	{
		return; 
	}
	
	ptr = (*_list)->m_head.m_next;

	/* NULL != ptr is not needed asuming we did all the rest well */
	while (NULL != ptr && ( &((*_list)->m_tail) != ptr) ) 
	{
		ptr = ptr->m_next;
		
		/* one can change to two option deapanding on _elementDestroy so wont do a check every time. more code copy, less resuource */
		if (NULL != _elementDestroy)
		{
			_elementDestroy (ptr->m_prev->m_data);
		}
		
		FreeNode(ptr->m_prev);
	}
	
	
	(*_list)->m_magicNumber = DEAD_MAGIC_NUMBER;

	free(*_list);
	
	*_list = NULL;
	
	return; 
}


List_Result List_PushHead(List* _list, void* _item)
{
	List_Result result;
	
	if (!CheckListParam(_list))
	{
		return LIST_UNITIALIZED_ERROR;
	}
	
	result = List_PushNode(_list->m_head.m_next, _item);
	
	return result;
}


List_Result List_PushTail(List* _list, void* _item)
{
	List_Result result;
	
	if (!CheckListParam(_list))
	{
		return LIST_UNITIALIZED_ERROR;
	}
	
	result = List_PushNode(&(_list->m_tail), _item);
	
	return result;
}


List_Result List_PopHead(List* _list, void** _pItem)
{
	List_Result result;

	if ( !CheckListParam(_list) || NULL == _pItem)
	{
		return LIST_UNITIALIZED_ERROR;
	}
	
	if (0 == List_Size(_list) )
	{
		return LIST_UNDERFLOW;
	}
	
	result = List_PopNode(_list->m_head.m_next, _pItem);
	
	return result;
}




List_Result List_PopTail(List* _list, void** _pItem)
{
	List_Result result;
	
	if (!CheckListParam(_list) || NULL == _pItem)
	{
		return LIST_UNITIALIZED_ERROR;
	}
	
	if (0 == List_Size(_list) )
	{
		return LIST_UNDERFLOW;
	}
	
	result = List_PopNode(_list->m_tail.m_prev, _pItem);
	
	return result;
}

/* TODO change name to List_PushNodeBefore */
static List_Result List_PushNode(Node_t* _node, void* _item)
{
	Node_t* new_node;
	
	/*if ( !CheckNodeParam(_node) )
	{
		return LIST_NODE_UNITIALIZED_ERROR;
	}*/
	if ( NULL == _item)
	{
		return LIST_DATA_NULL;
	}
	
	new_node = (Node_t*) malloc(sizeof(Node_t));
	if (NULL == new_node)
	{
		return LIST_ALLOCATION_ERROR;
	}
	
	new_node->m_data = _item;
	new_node->m_magicNumber = ALIVE_MAGIC_NUMBER;
	
	NodeInsertLinkerFix(new_node, _node->m_prev ); 
	
	
	return LIST_SUCCESS;
}

static List_Result List_PopNode(Node_t* _node, void** _pItem)
{
	
	/*if ( !CheckNodeParam(_node) )
	{
		return LIST_NODE_UNITIALIZED_ERROR;
	}*/
	
	*_pItem = _node->m_data;
	/* returning the void* of data. user responsibel to free it */
	
	NodeRemoveLinkerFix(_node);
	
	
	/* TODO magic number to dead */
	FreeNode(_node);
	
	
	return LIST_SUCCESS;
}





static void NodeInsertLinkerFix(Node_t* _node, Node_t* _before_node)
{
	
	Node_t* after_node = NULL;
	
	
	after_node = _before_node->m_next;
	
	_node->m_prev = _before_node;
	_node->m_next = after_node;
	
	_before_node->m_next = _node;
	after_node->m_prev = _node;
	
	return;
	
}

static void NodeRemoveLinkerFix(Node_t* _node)
{
	Node_t* node_before = 0;
	Node_t* node_after = 0;
	
	node_before = _node->m_prev;
	node_after = _node->m_next;
	
	node_before->m_next = node_after;
	node_after->m_prev = node_before;
	
	
	
	return;
	
}


size_t List_Size(const List* _list)
{ 
	size_t count = 0;
	Node_t* ptr = 0;
	
	if ( !CheckListParam(_list))
	{
		return 0;
	}
	
/* save end in value and check it to save resourse */
	ptr = _list->m_head.m_next;
	while (NULL != ptr && ( &(_list->m_tail) != ptr) )
	{
		++count;
		ptr = ptr->m_next;
	}

	return count;
}

void List_Print(List* _list, void (*_elementPrint)(void* _item))
{
	Node_t* ptr = 0;
	Uint_t count = 0;
	
	if ( !CheckListParam(_list) )
	{
		return;
	}
	
	printf("\nList of Node Print\n--------------------");
	
	ptr = _list->m_head.m_next;

	while (NULL != ptr && ( &(_list->m_tail) != ptr) )
	{
		++count;
		printf("\nNode %u:",count);
		
		if (NULL != _elementPrint)
		{
			_elementPrint (ptr->m_data);
		}
		
		ptr = ptr->m_next;
	}


	return;
}



static void FreeNode(Node_t* _node)
{
	
	if ( !CheckNodeParam(_node) )
	{
		return;
	}
	
	_node->m_magicNumber = DEAD_MAGIC_NUMBER;
	
	free(_node);
	return;
}


/* List ITR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

ListItr ListItr_Begin(const List* _list)
{
	if ( !CheckListParam(_list) )
	{
		return NULL;
	}
	
	return _list->m_head.m_next;
}


ListItr ListItr_End(const List* _list)
{
	if ( !CheckListParam(_list) )
	{
		return NULL;
	}
	
	
	return (ListItr) &(_list->m_tail);
}


bool ListItr_Equals(const ListItr _a, const ListItr _b)
{
	
	return (_a == _b);
}

ListItr ListItr_Next(ListItr _itr)
{
	Node_t* nextItr;
	
	nextItr = ((Node_t*)_itr)->m_next ;
	if (NULL == nextItr )
	{ /* reached the end, returning the end as recived */
		return _itr;
	}
	
	return nextItr;
}

ListItr ListItr_Prev(ListItr _itr)
{
	Node_t* prevItr;
	
	prevItr = ((Node_t*)_itr)->m_prev ;
	if (NULL == prevItr->m_prev )
	{ /* reached the end, returning the end as recived */
		return _itr;
	}
	
	return prevItr;
}

void* ListItr_Get(ListItr _itr)
{
	if ( !CheckNodeParam(_itr) )
	{
		return NULL; 
	}	

/*	 sentinal data needs to be null */
/*	if (NULL == ((Node_t*)_itr)->m_next)*/
/*	{  reached the end, dont deal with the sentinal */
/*		return NULL;*/
/*	}*/
	
	return ((Node_t*)_itr)->m_data;
	
}


void* ListItr_Set(ListItr _itr, void* _element)
{
	void* tempHolder;
	
	if ( !CheckNodeParam(_itr) || NULL == _element )
	{
		return NULL; 
	}
	
	if (NULL == ((Node_t*)_itr)->m_next)
	{ /* reached the end, dont deal with the sentinal */
		return NULL;
	}
	
	tempHolder = ((Node_t*)_itr)->m_data;
	((Node_t*)_itr)->m_data = _element;
	
	return tempHolder;
}

ListItr ListItr_InsertBefore(ListItr _itr, void* _element)
{
	List_Result result;
	if ( !CheckNodeParam(_itr) )
	{
		return NULL; 
	}
	
	
	result = List_PushNode( (Node_t*) _itr, _element);
	if (LIST_SUCCESS != result)
	{
		return NULL;
	}
	
	return ((Node_t*) _itr)->m_prev;
}



void* ListItr_Remove(ListItr _itr)
{
	List_Result result;
	void* data;
	
	if ( !CheckNodeParam(_itr) )
	{
		return NULL; 
	}
	
	/* do not remove sentinal end */
	if ( NULL == ((Node_t*)_itr)->m_next )
	{
		return NULL;
	}
	
	result = List_PopNode(_itr, &data);
	if (LIST_SUCCESS != result)
	{
		return NULL;
	}
	
	return data;
	
}

static bool CheckNodeParam(const Node_t* _node)
{
	if ( NULL == _node || ALIVE_MAGIC_NUMBER != ((Node_t*)_node)->m_magicNumber )
	{
		return FALSE; 
	}
	
	return TRUE;
}

static bool CheckListParam(const List_t* _list)
{
	if ( NULL == _list || ALIVE_MAGIC_NUMBER != (_list)->m_magicNumber )
	{
		return FALSE;
	}
	return TRUE;
}
	
ListItr ListItr_FindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	Node_t* ptr;
	
	if ( !CheckNodeParam(_begin) || !CheckNodeParam(_end) || NULL == _predicate)
	{
		return NULL; 
	}
	
	ptr = ((Node_t*)_begin);

	while (NULL != ptr && ( NULL != (*ptr).m_next) && ( _end != (ptr) ) ) 
	{
		
		if ( _predicate (ptr->m_data, _context) )
		{
			/* found TRUE, so returning the pointer */
			/* return (ListItr) ptr; */
			break;
		}
		
		ptr = ptr->m_next;
	}
	/* did not found TRUE, so returning the end pointer */
	return (ListItr) ptr;
}

/* TODO change all function to be an itr api. 
*	no null check beacouse i don;t know what is itr 
*	no derefrancfe becouse i dont know node.
*	so use only itr functions!
*/

size_t ListItr_CountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	Node_t* ptr;
	size_t count = 0;
	
	if ( !CheckNodeParam(_begin) || !CheckNodeParam(_end) || NULL == _predicate)
	{
		return 0; 
	}
	
	ptr = ((Node_t*)_begin);

	while (NULL != ptr && ( NULL != (*ptr).m_next) && ( _end != (ptr) ) ) 
	{
		
		if ( _predicate (ptr->m_data, _context) )
		{
			/* found TRUE */
			++count;
		}
		
		ptr = ptr->m_next;
		/* printf("\n ptr:%p. (*ptr).m_next):%p. _end:%p. ", ptr, (*ptr).m_next , _end ); */
	}

	return count;
}


ListItr ListItr_ForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
	Node_t* ptr;
	
	if ( !CheckNodeParam(_begin) || !CheckNodeParam(_end) || NULL == _action)
	{
		return NULL; 
	}
	
	ptr = ((Node_t*)_begin);

	while (NULL != ptr && ( NULL != (*ptr).m_next) && ( _end != (ptr) ) ) 
	{
		
		if (! _action (ptr->m_data, _context) )
		{
			/* found TRUE, so returning the pointer */
			return (ListItr) ptr;
		}
		
		ptr = ptr->m_next;
		/* printf("\n ptr:%p. (*ptr).m_next):%p. _end:%p. ", ptr, (*ptr).m_next , _end ); */
	}
	/* did not found TRUE, so returning the end pointer */
	return _end;
}

/* TODO there might be a bug where _begin & _end are just consecative. check it */
void ListItr_Sort(ListItr _begin, ListItr _end, LessFunction _less) /* TODO cleanup code */
{
	Node_t* ptr;
	Node_t* ptrNext;
	Node_t* internalPtr;
	Node_t* internalPtrNext;
	bool swapFlag;
	
	if ( !CheckNodeParam(_begin) || !CheckNodeParam(_end) || NULL == _less)
	{
		return; 
	}
	if (_begin == _end)
	{
		/* nothing to do */
		return;
	}
	
	ptr = ((Node_t*)_begin);
	ptrNext = ptr->m_next;

	while (	NULL != ptrNext && ( NULL != ptrNext->m_next) && ( _end != (ptrNext) ) ) 
	{
		swapFlag = FALSE;

		internalPtr = ((Node_t*)_begin);
		internalPtrNext = internalPtr->m_next;
		
		while ( NULL != internalPtrNext && ( NULL != internalPtrNext->m_next) && ( _end != (internalPtrNext) ) ) 
		{
			if ( _less (internalPtr->m_data, internalPtrNext->m_data) )
			{
				/* found TRUE, so swap the pointer */
				swapFlag = swapPtr(&(internalPtr->m_data), &(internalPtrNext->m_data) );
				/* swag flag set to true */
			}
			
			internalPtr = internalPtrNext;
			internalPtrNext = internalPtrNext->m_next;
		}
		ptr = ptrNext;
		ptrNext = ptrNext->m_next;
		
		/* every run the last item is inplace and does not need to be checked */
		_end = ((Node_t*)_end)->m_prev;

		/* check swapFlag for no need to contine sort */
		if (FALSE == swapFlag)
		{
			break;
		}
	}
	
	
	return;
}


static bool swapPtr(void** first, void** second)
{
	void* tempHolder;
	tempHolder = *first;
	*first = *second;
	*second = tempHolder;
	
	return TRUE;
}


ListItr ListItr_Splice(ListItr _dest, ListItr _begin, ListItr _end)
{
	
	if ( !CheckNodeParam(_begin) || !CheckNodeParam(_end) || !CheckNodeParam(_dest) )
	{
		return NULL; 
	}
	
	MoveNodes ( _dest, _begin, _end);
	
	return _end;
	
}


ListItr ListItr_Merge(ListItr _destBegin, ListItr _firstBegin, ListItr _firstEnd, ListItr _secondBegin, ListItr _secondEnd, LessFunction _less)
{
	Node_t* ptrA;
	Node_t* ptrB;
	
	if ( !CheckNodeParam(_destBegin) || !CheckNodeParam(_firstBegin) || !CheckNodeParam(_firstEnd) || !CheckNodeParam(_secondBegin) || !CheckNodeParam(_secondEnd) )
	{
		return NULL; 
	}
	
	
	if (NULL == _less)
	{
		MoveNodes ( _destBegin, _firstBegin, _firstEnd);
		MoveNodes ( _destBegin, _secondBegin, _secondEnd);
	}
	else
	{
		/* TODO why not just move both list and at the end sort the dest in the corrent values */
		
		
		/* bubbleSort insito first list */
		ListItr_Sort(_firstBegin, _firstEnd, _less);
		
		/* bubbleSort insito second list */
		ListItr_Sort(_secondBegin, _secondEnd, _less);
		/* go throu each item, compare them using _less and desice which list to copy one by one with MoveNodes */
		
		ptrA = (Node_t*) _firstBegin;
		ptrB = (Node_t*) _secondBegin;
		
		while (ptrA != _firstEnd || ptrB != _secondEnd)
		{
			if (ptrA == _firstEnd && ptrB != _secondEnd)
			{
				MoveNodes (_destBegin, ptrB, _secondEnd);
				ptrB = _secondEnd;
			}
			else if (ptrA != _firstEnd && ptrB == _secondEnd)
			{
				MoveNodes (_destBegin, ptrA, _firstEnd);
				ptrA = _secondEnd;
			}
			
			else if ( _less (ptrA->m_data, ptrB->m_data) )
			{
				ptrB = MoveData (_destBegin, ptrB);
				
			}
			else
			{
				ptrA = MoveData (_destBegin, ptrA);
			}
		}
	}
	return _destBegin;
}



List* ListItr_Cut(ListItr _begin, ListItr _end)
{
	List* newList;
	
	if ( !CheckNodeParam(_begin) || !CheckNodeParam(_end) )
	{
		return NULL; 
	}
	
	newList = List_Create();
	
	MoveNodes ( (ListItr)&(newList->m_tail), _begin, _end);
	
	return newList;
}


List* ListItr_Unique(ListItr _begin, ListItr _end, EqualsFunction _equals)
{
	Node_t* ptr;
	Node_t* internalPtr;
	List* newList;
	
	if ( !CheckNodeParam(_begin) || !CheckNodeParam(_end) || NULL == _equals)
	{
		return NULL; 
	}
	
	newList = List_Create();
	
	ptr = ((Node_t*)_begin);

	while (	NULL != ptr && ( NULL != ptr->m_next) && ( _end != (ptr) ) ) 
	{
		internalPtr = ptr->m_next;
		
		while ( NULL != internalPtr && ( NULL != internalPtr->m_next) && ( _end != (internalPtr) ) ) 
		{
			if ( _equals (ptr->m_data, internalPtr->m_data) )
			{
				/* found TRUE, remove the internalPtr */
				MoveData ((ListItr)&(newList->m_tail), internalPtr);
			}
			
			internalPtr = internalPtr->m_next;
		}
		ptr = ptr->m_next;
	}
	
	return newList;
}

static void MoveNodes (ListItr _dest, ListItr _begin, ListItr _end)
{

	Node_t* ptr;
	
	ptr = ((Node_t*)_begin);

	while (NULL != ptr && ( NULL != (*ptr).m_next) && ( _end != (ptr) ) ) 
	{
		ptr = MoveData (_dest, ptr);
		
		/* printf("\n ptr:%p. (*ptr).m_next):%p. _end:%p. ", ptr, (*ptr).m_next , _end ); */
	}
	
	return;
}

static Node_t* MoveData (ListItr _dest, ListItr _source)
{
	void* dataHolder;
	Node_t* ptrTemp;
	
	ptrTemp = ((Node_t*) _source)->m_next;
	
	dataHolder = ListItr_Remove(_source);
	ListItr_InsertBefore(_dest, dataHolder);
	
	return ptrTemp;
}





