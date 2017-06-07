#ifndef __Vector_Result_h__
#define __Vector_Result_h__

typedef enum Vector_Result {
	VECTOR_SUCCESS = 0,
	VECTOR_UNITIALIZED_ERROR,				/**< Uninitialized vector error 	*/
	VECTOR_ALLOCATION_ERROR,				/**< realloc error on grow/shrink   */
	VECTOR_INDEX_OUT_OF_BOUNDS_ERROR,
	VECTOR_OVERFLOW,
	VECTOR_UNDERFLOW,
	VECTOR_REALLOCATION_FAILED,
	VECTOR_WRONG_INDEX,
	VECTOR_UNKNOWN_ERROR
} Vector_Result;



#endif/* __Vector_Result_h__ */
