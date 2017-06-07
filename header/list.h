#ifndef __LIST_H__
#define __LIST_H__

/** 
 *  @file list.h
 *  @brief Create a Generic Double Linked List data type
 * 
 *  @details Implemented as double linked list using head and tail sentinels.
 *  Nodes are allocated from heap an store pointer to user proveded element.
 * 
 *  @author Author MuhammadZ (muhammadz@experis.co.il) 
 *			updated by Yuval Hamberg Mar 2017
 * 
 *  @bug No known bugs.
 */ 

#include <stddef.h>  /* for size_t */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* printf for print function and debug mode  */

#include "list_result.h"
#include "defines.h"


typedef struct List List;
typedef List List_t;
typedef int	(*ListElementAction)(void* _element, size_t _index, void* _context);

/* comment out to turn on DeBug mode */
#define NDEBUG

/** 
 * @brief Create a list
 * 
 * @returns a pointer to the created list.
 * @retval NULL on failure due to allocation failure
 */
List* List_Create(void);

/** 
 * @brief Destroy list
 * @details Destroys the list completely 
 *          optionally destroys elements using user provided function
 *
 * @params[in] _pList : A pointer to previously created List returned via ListCreate
 *					   on completion *_pList will be null
 * @params[in] _elementDestroy : A function pointer to be used to destroy elements inserted into the list
 *             or a null if no such destroy is required
 */
void List_Destroy(List** _pList, void (*_elementDestroy)(void* _item));

/** 
 * @brief Add element to head of list
 * @details time complexity: O(1).
 *
 * @params _list A previously created List ADT returned via ListCreate
 * @params _item An item to add to the list
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized
 * @retval LIST_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval LIST_ALLOCATION_ERROR on memory allocation failure
 */
List_Result List_PushHead(List* _list, void* _item);

/** 
 * @brief Add element to the list's tail
 * @details time complexity O(1).
 *
 * @params _list pointer to the list
 * @params _data void pointer to data to be insered.
 * @returns LIST_UNITIALIZED_ERROR if list is null or destroyed. 
 * @returns LIST_NODE_UNITIALIZED_ERROR if one of the node is null or destroyed.
 * @returns LIST_ALLOCATION_ERROR if malloc failed.
 * @retval 
 * @retval 
 */
List_Result List_PushTail(List* _list, void* _item);

/** @brief Remove element from list's head
 *  @details if successfull, stores a pointer to the removed item in _item
 *  time complexity O(1).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @params _pItem: To store a pointer to the removed item in
 * @returns LIST_UNITIALIZED_ERROR if list is null or destroyed. 
 * @returns LIST_NODE_UNITIALIZED_ERROR if one of the node is null or destroyed.
 * @returns LIST_ALLOCATION_ERROR if malloc failed.
 * @retval 
 */
List_Result List_PopHead(List* _list, void** _pItem);

/** @brief Remove element from list's tail
 *  @details if successfull, stores a pointer to the removed item in _item
 *  time complexity O(1).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @params _pItem: To store a pointer to the removed item in

 * @returns LIST_UNITIALIZED_ERROR if list is null or destroyed. 
 * @returns LIST_NODE_UNITIALIZED_ERROR if one of the node is null or destroyed.
 * @returns LIST_ALLOCATION_ERROR if malloc failed.
 * @retval 
 */
List_Result List_PopTail(List* _list, void** _pItem);

/** @brief Get number of elements in the list
 * Average time complexity O(n).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * get this number by running on all nodes.
 *
 * failer would return 0 --yuval.
 */
size_t List_Size(const List* _list);



/** @brief Print all the elements in the list
 *	Uses user function to get to void* data.
 * Average time complexity O(n).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @params _elementPrint: user function to get to void* data.
 *
 * if list in null nothing would be reutrned. --yuval.
 */
void List_Print(List* _pList, void (*_elementPrint)(void* _item));


#endif /* __LIST_H__ */
