#ifndef __list_result_h__
#define __list_result_h__

typedef enum List_Result {
	OK = 0,
	LIST_SUCCESS = 0,
	LIST_UNITIALIZED_ERROR,				/**< Uninitialized LIST error 	*/
	LIST_ALLOCATION_ERROR,				/**< realloc error on grow/shrink   */
	LIST_INDEX_OUT_OF_BOUNDS_ERROR,
	LIST_OVERFLOW,
	LIST_UNDERFLOW,
	LIST_REALLOCATION_FAILED,
	LIST_WRONG_INDEX,
	LIST_NODE_UNITIALIZED_ERROR,
	LIST_DATA_NULL
	
} List_Result;



#endif/* __list_result_h__ */
