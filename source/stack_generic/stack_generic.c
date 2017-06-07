#include "list.h"
#include "stack_generic.h"


/*	This libary builds and oprate data stratue of stack type.
// 	on top of list_generic.a libray
//	
//	Yuval Hamberg March 2017
//	yuval.hamberg@gmail.com
 */



Stack* StackCreate(void)
{
	return List_Create();
	
}


void StackDestroy(Stack** _pStack, void (*_elementDestroy)(void* _item))
{
	List_Destroy(_pStack, _elementDestroy);
	return;
}


List_Result StackPush(Stack* _stack, void*  _item)
{
	return List_PushHead(_stack, _item);
}


List_Result  StackPop    (Stack* _stack, void** _item)
{
	return List_PopHead(_stack, _item);
}


List_Result  StackTop    (Stack* _stack, void** _item)
{
/*	list lib doesn't have get or peek function.*/
/*	therefor, this function removes the head, get the needed valuse and pushes it back at the same spot*/
	
	List_Result result;
	result = List_PopHead(_stack, _item);
	if (LIST_SUCCESS != result)
	{
		return result;
	}
	
	return List_PushHead(_stack, *_item);
}


bool	StackIsEmpty(Stack* _stack)
{
	if ( 0 == List_Size(_stack) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void StackPrint(Stack* _stack, void (*_elementPrint)(void* _item))
{
	List_Print(_stack, _elementPrint);
	return;
}


