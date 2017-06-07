#ifndef __HEAP_H__
#define __HEAP_H__

/** 
 * @brief Create a Binary heap generic data type over a generic vector data type.
 * @author Author MuhammadZ (muhammadz@experis.co.il)
 * @see https://en.wikipedia.org/wiki/Binary_heap 
 * Updated Yuval Hamberg March 2017 
 */ 

#define NDEBUG  /* remove to turn on debug printout */

#include <math.h>
#include <stdlib.h>
#include "defines.h"
#include "vector_generic.h"


#define PARENT i/2
#define LEFT_CHILD 2*i
#define RIGHT_CHILD 2*i+1

#define CHECK_RESULT_ERROR if(OK != result) { return result; }
#define CHECK_RESULT_ERROR_PRINT if(OK != result) { printf("\nERROR %d\n", result); return result; }
#define CHECK_RESULT_ERROR_NO_OUTPUT if(OK != result) { printf("\nERROR %d\n\n",result); return; }

typedef enum Heap_Result {
	HEAP_SUCCESS = 0,
	HEAP_NOT_INITIALIZED,
	HEAP_REALLOCATION_FAILED
} Heap_Result;

typedef struct Heap Heap;
typedef struct Heap Heap_t;

typedef int	(*ActionFunction)(const void *_elem, void * _context);
typedef int	(*LessThanComparator)(const void *_left, const void *_right);
typedef void (*SimpleJobFunc)(void* _item);

/**  
 * @brief Dynamically create a new heap  
 * @param[in] _vector - Vector that hold the elements, must be initialized
 		would not create if vector is empty. -- yuval
 * @param[in] _pfLess - A less than comparator to be used to compare elements. 
 True if _left is bigger than _right. --yuval
 * @return Heap * - on success
 * @retval NULL on fail 
 *	
 * @warning allocating and freeing the underlying vector is user responsibility. 
 */
Heap* Heap_Build(Vector* _vector, LessThanComparator _pfLess);

/**  
 * @brief Deallocate a previously allocated heap
 * Frees the heap but not the underlying vector  
 * @param[in] _heap - Heap to be deallocated.  On success will be nulled.
 * @return Underlying vector
 */
Vector* Heap_Destroy(Heap** _heap);

/**  
 * @brief Add an element to the Heap preserving heap property.  
 * @param[in] _heap - Heap to insert element to.
 * @param[in] _element - Element to insert.
 * @return success or error code 
 * @retval HEAP_OK  on success
 * @retval HEAP_NOT_INITIALIZED  error, heap not initilized
 * @retval HEAP_REALLOCATION_FAILED error, heap could not reallocate underlying vector 
 */
Heap_Result Heap_Insert(Heap* _heap, void* _element);

/**  
 * @brief Peek at element on top of heap without extracting it.  
 * @param[in] _heap - Heap to peek to.
 * @return pointer to element, can be null if heap is empty. 
 */
const void* Heap_Peek(const Heap* _heap); 

/**  
 * @brief Extract element on top of heap and remove it.  
 * @param[in] _heap - Heap to extract from.
 * @return pointer to element, can be null if heap is empty. 
 */
void* Heap_Extract(Heap* _heap);

/**  
 * @brief Get the current size (number of elements) in the heap.  
 * @param[in] _heap - Heap to extract from.
 * @return number of elements or zero if empty. 
 */
size_t Heap_Size(const Heap* _heap);

/**  
 * @brief Iterate over all elements  in the heap from top to bottom.
 * @details The user provided ActionFunction _act will be called for each element.  
 * @param[in] _heap - Heap to iterate over.
 * @param[in] _act - User provided function pointer to be onvoked for each element
 * @returns number of times the user functions was invoked
 */
size_t Heap_ForEach(const Heap* _heap, ActionFunction _act, void* _context);


/**  
 * @brief Sort a given vector of elments using a heap sort.
 * @param[in] _vector - vector to sort.
 * @param[in] _pfLess
 * @return number of elements or zero if empty. 
 */
 /* floyd or willians */
 /* dont forget to do build before sort */
void Heap_Sort(Vector* _vec, LessThanComparator _pfLess);


void Heap_Print(Heap_t* _heap, SimpleJobFunc _printItem);

void Heap_Print_Graphic(Heap_t* _heap, SimpleJobFunc _printItem);

#endif /*__HEAP_H__*/
