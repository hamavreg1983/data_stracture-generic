#include <time.h>
#include "heap_tree_generic.h"
/*Vector is needed here just becouse of user create a vector for the heap*/
#include "vector_generic.h" 
#include "defines.h" 

#define INIT_SIZE 10 /* size of starting array */
#define NUMOFELEM 15 /* how many random numbers in vector */

/*	A binary heap tree on top of vector libarery.
//	Testing File.
//	
//	Yuval Hamberg March 2017
//	yuval.hamberg@gmail.com
 */
 


int* CreateINT(int _value);
void IsTestPassed(char _is_equal, int _expected, int _result);
void ItemPrint(void* _item);
bool IsFirstItemBigger(const void *_left, const void *_right); /* typedef LessThanComparator */
void DestroyInt(void* _item);

int main()
{
	
	Vector* a_vec;
	Heap_t* heap;
	Uint_t _initialSize = INIT_SIZE;
	Uint_t _extensionBblockSize = _initialSize / 2 ;
	Uint_t _initialElemntAmount = NUMOFELEM;
	Vector_Result result;
	int i;
	void* max;
	const void* maxConst;
	size_t size;
	
	srand(time(NULL)); /* turn on to get real random numbers */
	
	
	a_vec = Vector_Create(_initialSize, _extensionBblockSize); 
	
	for (i = 1; i < _initialElemntAmount; ++i)
	{
		result = Vector_Append(a_vec, CreateINT(rand()%100) ); 
		if (VECTOR_SUCCESS != result)
		{
			printf("\nERROR %d", result);
			return result;
		}
	}
	VectorPrint(a_vec, (VectorElementAction) ItemPrint); 
	printf("\n Vector is ready\n");
	
	heap = Heap_Build(a_vec, IsFirstItemBigger);
	
	Heap_Print(heap, ItemPrint);
	
	
	Heap_Print_Graphic(heap, ItemPrint);
	
	
	printf("\nIn heap there are %u items.", Heap_Size(heap) );
	
	maxConst = Heap_Peek(heap);
	printf("\nMAX in heap is %d.", *(int*)maxConst );

	max = (void*) 0; /*put garbe just to see it change */
	 
	max = Heap_Extract(heap);
	if (NULL != max)
	{
		printf("\n removed MAX in heap is %d.", *(int*)max );
	}
	else
	{
		printf("\n removed MAX in heap GOT NULL." );
	}
	
	putchar('\n');
	Heap_Print_Graphic(heap, ItemPrint);
	

	printf("\nInserting 3 items");
	result = Heap_Insert(heap, CreateINT(-3) );
	result = Heap_Insert(heap, CreateINT(50) );
	result = Heap_Insert(heap, CreateINT(999) );
	putchar('\n');
	Heap_Print(heap, ItemPrint);

	printf("\nGraphical print\n");
	Heap_Print_Graphic(heap, ItemPrint);
	
	printf("\n Print using foreach");
	size = Heap_ForEach(heap, (ActionFunction) ItemPrint, NULL);
	printf("\n got %u items travarse throu.",size);
	
	
	
	
	Heap_Destroy(&heap);
	Heap_Destroy(&heap); /* double destroy test */
	
	
	
	printf("\n\n Sort:\n");
	Heap_Sort(a_vec, IsFirstItemBigger);  /* TODO bugs lay here, maybe */
	VectorPrint(a_vec , (VectorElementAction) ItemPrint );
	
	Vector_Destroy(&a_vec, DestroyInt);  
	
	printf("\n\n---END---\n");
	
	return VECTOR_SUCCESS;
}


void ItemPrint(void* _item)
{
	if (NULL == _item)
	{
		return;
	}
	
	printf(" %d",*(int *) _item);
	
	return;
}

bool IsFirstItemBigger(const void *_left, const void *_right)
{
	if (*(int*)_left > *(int*)_right)
	{
		return TRUE;
	}
	return FALSE;
}

void DestroyInt(void* _item)
{
	#if !defined(NDEBUG) /* PrintArray flag */
/*	printf("\n gonna destroy item with this value: %d,",*(int*)_item );*/
	#endif
	
	/* casting is not mandatory but help with readablety */

	free( (int*) _item); 
	
	return;
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


