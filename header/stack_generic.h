#ifndef __STACK_H__
#define __STACK_H__

#include "list.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*	This libary builds and oprate data stratue of stack type, genric data type.
// 	on top of list_generic.a libray
//	
//	Yuval Hamberg March 2017
//	yuval.hamberg@gmail.com
 */

typedef List Stack;




Stack* StackCreate(void);

void StackDestroy(Stack** _pStack, void (*_elementDestroy)(void* _item));

List_Result  StackPush   (Stack* _stack, void*  _item);

List_Result  StackPop    (Stack* _stack, void** _item);

List_Result  StackTop    (Stack* _stack, void** _item);

bool     StackIsEmpty(Stack* _stack);

void StackPrint(Stack* _stack, void (*_elementPrint)(void* _item));




#endif/* __STACK_H__ */

