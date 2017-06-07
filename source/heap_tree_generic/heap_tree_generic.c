#include <stdlib.h>
#include <math.h>
#include "heap_tree_generic.h"
#include "vector_generic.h"
#include "defines.h"

#define ALIVE_NUM 0xbeefbeef
#define DEAD_NUM 0xdeadbeef


/*	A binary heap tree on top of vector libarery
//	
//	Yuval Hamberg March 2017
//	yuval.hamberg@gmail.com
 */

struct Heap
{
	int m_alive_magicNumber;
	Vector* m_vec;
	size_t m_heapSize; 
	LessThanComparator m_func;
};


/* Testing function */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


/* Forward Declartion ************* */

/* main algorithem of the heap tree. i is starting index to work. */
/*	A recursive function to sort the heap so max on top. all childern would be smaller than parents.
//	recive a vector pointer, the index to work on, and the total number of items in vector to prevent overflow.
//	does not return anything. */
static void Heapify(Heap* _heap, size_t i, size_t _n_of_item);

/* a smaller version of Heapify, used to bring item from the bottom of the tree to thier right place */
static void BubbleUP(Heap* _heap, Uint_t i, size_t _n_of_item);

/* verifiy heap pointer is not null and alive magic number. return TRUE if there is a problame */
static bool CheckHeapParam(const Heap* _heap);

/* Functions ********************** */

Heap* Heap_Build(Vector* _vec, LessThanComparator _pfLess)
{
	Heap_t* a_heap;
	Uint_t n_item;
	size_t i;
	
	if (NULL == _vec || NULL == _pfLess)
	{
		return NULL;
	}
	
	n_item = Vector_Size(_vec);
	if (0 == n_item)
	/* empty or faulty vector, nothing to do */
	{
		return NULL;
	}
	
	a_heap = (Heap_t*) calloc(1,sizeof(Heap_t));
	if (NULL == a_heap)
	{
		return NULL;
	}
	
	a_heap->m_vec = _vec;
	a_heap->m_func = _pfLess;
	a_heap->m_heapSize = n_item; 
	a_heap->m_alive_magicNumber = ALIVE_NUM;
	
	i=(a_heap->m_heapSize);
	for(i=PARENT; i > 0  ; --i) 
	{
		Heapify(a_heap, i, Heap_Size(a_heap));
	}
	return a_heap;
}


/*	A recursive function to sort the heap so max on top. all childern would be smaller than parents.
//	recive a vector pointer, the index to work on, and the total number of items in vector to prevent overflow.
//	does not return anything. */
static void Heapify(Heap* _heap, size_t i, size_t _n_of_item)
{
	void* parent;
	void* left_c;
	void* right_c;
	void* largest_num;
	int largest_index;
/*	Vector_Result result;*/
	Vector* _vec;
	LessThanComparator _pfLess;
	
	if ( 2 > Heap_Size(_heap) )
	{
		#if !defined(NDEBUG) 
			printf("\n ERROR? Heapify with less than 2 items:%d\n", Heap_Size(_heap) );
		#endif
		return;
	}
	
	_pfLess = _heap->m_func;
	_vec = _heap->m_vec;
	
/*	printf("\nRec i=%u.",i);*/
	
	Vector_Get(_vec, i, &parent); 
	Vector_Get(_vec, LEFT_CHILD, &left_c);
	Vector_Get(_vec, RIGHT_CHILD, &right_c);
	
	#if !defined(NDEBUG) 
	printf(KCYN "\n checking child? %d with parent %d" KNRM, LEFT_CHILD , i);
	#endif
	if (LEFT_CHILD <= _n_of_item && _pfLess(left_c, parent) ) /* left chiled is the bigest */
	{
		largest_num = left_c;
		largest_index = LEFT_CHILD;
	}
	else /* parent is the biggest */
	{ 
		largest_num = parent;
		largest_index = i;
	}
	if (RIGHT_CHILD <= _n_of_item && _pfLess(right_c, largest_num) )
	{
		largest_num = right_c;
		largest_index = RIGHT_CHILD;
	}
	
	if (largest_num != parent)
	{
	
		Vector_Set(_vec, i, &largest_num);
		Vector_Set(_vec, largest_index, &parent);
		
		Heapify(_heap, largest_index, _n_of_item);
	}
	
	
	return;
}

static void BubbleUP(Heap* _heap, Uint_t i, size_t _n_of_item)
{
	Vector_Result result;
	void* parent;
	void* child;
	
	Vector* _vec = _heap->m_vec;
	LessThanComparator _pfLess = _heap->m_func;

/*	printf("\n in bubbleup with i:%d _n_of_item:%d", i , _n_of_item);*/
	/* stoping condition */
	if (0 >= PARENT || _n_of_item < i) 
	{ 
		return;
	}
	
	result = Vector_Get(_vec, i, &child); 
	result |= Vector_Get(_vec, PARENT, &parent);
	if(VECTOR_SUCCESS != result) { printf(RED "\n 2ERROR BubbleUP %d.",result); return; }
/*	CHECK_AND_RETURN_VOID (result, !VECTOR_SUCCESS);*/
	
	#if !defined(NDEBUG) 
	printf(KCYN "\n checking child %d with parent %d" KNRM, i , PARENT);
	#endif
	
	if (_pfLess(child, parent) ) /* chiled is the biger */
	{
/*		printf(" decided to swap %d with %d", i , PARENT);*/
		/*swap */
		Vector_Set(_vec, i, &parent);
		Vector_Set(_vec, PARENT, &child);
		
		/* recall bubble*/
/*		BubbleUP(_heap, LEFT_CHILD, _n_of_item);*/
/*		BubbleUP(_heap, RIGHT_CHILD, _n_of_item);*/
		
		if (0 != i)
		{
			BubbleUP(_heap, PARENT, _n_of_item);
		}
	}
	
	/* parent is the biger -> end */
	return;
}


/**  
 * @brief Deallocate a previously allocated heap
 * Frees the heap but not the underlying vector  
 * @param[in] _heap - Heap to be deallocated.  On success will be nulled.
 * @return Underlying vector
 */
Vector* Heap_Destroy(Heap** _heap)
{
	Vector* vecTempHolder;
	
	if ( NULL == _heap || CheckHeapParam(*_heap) )
	{
		return NULL;
	}
	
	vecTempHolder = (*_heap)->m_vec;
	
	(*_heap)->m_vec = NULL;
	(*_heap)->m_heapSize = 0;
	(*_heap)->m_alive_magicNumber = DEAD_NUM;
	
	free(*_heap);
	
	*_heap = NULL;
	
	return vecTempHolder;
}





/**  
 * @brief Add an element to the Heap preserving heap property.  
 * @param[in] _heap - Heap to insert element to.
 * @param[in] _element - Element to insert.
 * @return success or error code 
 * @retval HEAP_HEAP_SUCCESS  on success
 * @retval HEAP_NOT_INITIALIZED  error, heap not initilized
 * @retval HEAP_REALLOCATION_FAILED error, heap could not reallocate underlying vector 
 */
Heap_Result Heap_Insert(Heap* _heap, void* _element)
{
	Vector_Result result;
	Uint_t i;
	
	if ( CheckHeapParam(_heap) )
	{
		return HEAP_NOT_INITIALIZED;
	}
	
	result = Vector_Append(_heap->m_vec, _element); 
	if(VECTOR_SUCCESS != result) { return HEAP_REALLOCATION_FAILED; }
	
	++(_heap->m_heapSize);
	
	
	/* place new element in order */
	i = _heap->m_heapSize;
	BubbleUP(_heap, i, Heap_Size(_heap) );
	
	return HEAP_SUCCESS;
}


size_t Heap_Size(const Heap* _heap)
{
	/* TODO shold calulate number? */
	if ( CheckHeapParam(_heap) )
	{
		return 0;
	}
	
	return (_heap->m_heapSize);
}

const void* Heap_Peek(const Heap* _heap)
{
	Vector_Result result;
	void* dataHolder;
	
	if ( CheckHeapParam(_heap) )
	{
		return NULL;
	}
	
	result = Vector_Get(_heap->m_vec, 1, &dataHolder);
	if(VECTOR_SUCCESS != result) { return NULL; }

	return dataHolder;
}

void* Heap_Extract(Heap_t* _heap)
{
	Vector_Result result;
	void* temp;
	void* item;
	void* dataHolder;
	size_t index = 1;
	
	if ( CheckHeapParam(_heap) )
	{
		return NULL;
	}
	
	/* Heap_Peek returns a const void* , but now the void* belong to the user and he can change it */
	dataHolder = (void*) Heap_Peek(_heap);
	
	/* if heap is empty dataHolder would be NULL */
	if(NULL == dataHolder) 
	{ 
		return NULL; 
	}
	
	/* move last item to first */
	result = Vector_Get(_heap->m_vec, _heap->m_heapSize, &item); 
	result |= Vector_Set(_heap->m_vec, index, &item); 
	result |= Vector_Remove(_heap->m_vec, &temp); 
	if(VECTOR_SUCCESS != result) { printf(RED "\n 2 ERROR Heap_Extract %d", result) ; return NULL; }
	
	/* lower number of item */
	--(_heap->m_heapSize);

	Heapify(_heap, 1, Heap_Size(_heap) );
	
	return dataHolder;
}


/**  
 * @brief Iterate over all elements  in the heap from top to bottom.
 * @details The user provided ActionFunction _act will be called for each element.  
 * @param[in] _heap - Heap to iterate over.
 * @param[in] _act - User provided function pointer to be onvHEAP_SUCCESSed for each element
 * @returns number of times the user functions was invHEAP_SUCCESSed
 */
size_t Heap_ForEach(const Heap* _heap, ActionFunction _act, void* _context)
{
	return Vector_ForEach(_heap->m_vec, (VectorElementAction) _act, _context);
}


/**  
 * @brief Sort a given vector of elments using a heap sort.
 * @param[in] _vector - vector to sort.
 * @param[in] _pfLess
 * @return number of elements or zero if empty. 
 */
 /* floyd or willians */
 /* dont forget to do build before sort */
void Heap_Sort(Vector* _vec, LessThanComparator _pfLess)
{
	/* TODO a better algorithem would be to seaprate extractMax to sub function and call them: getmax, swap, heapify */
	
	Heap* sortHeap;
	void* tempMax;
	int i;
	
	sortHeap = Heap_Build(_vec, _pfLess);
	tempMax = (void*) Heap_Peek(sortHeap);
	
	for (i = Heap_Size(sortHeap) ; i > 0 ; --i)
	{
				
		Vector_Append(sortHeap->m_vec, tempMax); /* add something just to remove it a row later */
		tempMax = Heap_Extract(sortHeap);
		Vector_Set(sortHeap->m_vec, i, &tempMax);
	}
	return;
}



static bool CheckHeapParam(const Heap* _heap)
{
	return ( NULL == _heap || ALIVE_NUM != _heap->m_alive_magicNumber );
	/* return TRUE if values are not correct */
}

void Heap_Print(Heap_t* _heap, SimpleJobFunc _printItem)
{
	if (CheckHeapParam(_heap) || NULL == _printItem)
	{
		return;
	}
	
	VectorPrint(_heap->m_vec , (VectorElementAction) _printItem );
	
	return;
}

void Heap_Print_Graphic(Heap_t* _heap, SimpleJobFunc _printItem)
{
	/* TODO do a nicer print? */
	
	Vector_Result result;
	int i, j,n_spaces;
	void* item; 
	int number_of_item;
	double line_num;
	
	if (CheckHeapParam(_heap))
	{
	return;
	}
	
	number_of_item = Heap_Size(_heap);
	line_num = 3 * sqrt(10);
	
	for(i=1, j=1; i <= number_of_item ;++i)
	{
		result = Vector_Get(_heap->m_vec, i, (void**) &item);
		if(VECTOR_SUCCESS != result) { printf("ERROR %d",result); return; }
		
		
		
		if (!(i % j))
		{
			j = j * 2;
			line_num = line_num-1;
			putchar('\n');
			for (n_spaces = 3* (number_of_item-i); n_spaces > 0; --n_spaces)
			{
				printf(" ");
			}
		}
		
		_printItem(item);
		
		for (n_spaces = (line_num); n_spaces > 0; --n_spaces)
		{
			printf(" ");
		}
		
	}
	return;
}
