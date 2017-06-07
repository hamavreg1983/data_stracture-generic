#include "vector_generic.h"
#include <time.h>


#define CHECK_STATUS_ERROR_PRINT if(VECTOR_SUCCESS != status) { printf("\nERROR %d\n", status); return status; }

#define INIT_SIZE 10
#define BLOCK_SIZE 50

/*	Generic type vector libary. builds and oprate void pointers data stratue of stack type.
//	Testing file.
//	
//	Yuval Hamberg Mar 2017
//	yuval.hamberg@gmail.com
 */

Vector_t* SetUP(int _numOfItems); /* setup an vector array on rand int srand is off */
int* CreateINT(int _value);

void DestroyInt(void* _item);
bool PrintItemInt(void* _item, size_t _index, void* _context);


/* global */
Vector_t* testing_vector;

void DestroyInt(void* _item)
{
	#if !defined(NDEBUG) /* PrintArray flag */
/*	printf("gonna destroy item with this value: %d,",*(int*)_item );*/
	#endif
	
	/* casting is not mandatory but help with readablety */

	free( (int*) _item); 
	
	return;
}

bool PrintItemInt(void* _item, size_t _index, void* _context)
{
	if (NULL == _item)
	{
		return FALSE;
	}
	
	printf(" %d,",*(int*)_item );
	return TRUE;
}

int main()
{
	size_t initialCapacity = INIT_SIZE;
	size_t blockSize = BLOCK_SIZE;
	Vector_t* vectorPtr;
	Vector_Result status;
	int holder =0;
	int* holderPTR = &holder;
	int i;
	int value = 2;
	
	vectorPtr = Vector_Create(initialCapacity, blockSize);
	
	for (i=0; i<3; ++i)
	{
		status = Vector_Append(vectorPtr, (void*) CreateINT(value += 2) );
		status |= Vector_Append(vectorPtr, (void*) CreateINT(value *= -2) );
		CHECK_STATUS_ERROR_PRINT
	}
	
	
	printf("\n before\t%d in item.", holder);
	status = Vector_Get(vectorPtr,1, (void**) &holderPTR);
	CHECK_STATUS_ERROR_PRINT
	printf("\n got \t%d in item.", *(int*)holderPTR);
	
	printf("\n Print vector: ");
	VectorPrint(vectorPtr, PrintItemInt ); 
	
	status = Vector_Remove(vectorPtr, (void**) &holderPTR);
	CHECK_STATUS_ERROR_PRINT
	printf("\n got \t%d in item when removed.", *(int*)holderPTR);
	
	printf("\n Size of vector is %u elements spaces.",Vector_Size(vectorPtr) );
	printf("\n and conteans %u elements in vector.",Vector_Capacity(vectorPtr) );
	
	
	holderPTR = CreateINT(77);
	status = Vector_Set(vectorPtr, 3, (void**) &holderPTR);
	CHECK_STATUS_ERROR_PRINT
	printf("\n got back \t%d when set.", *(int*)holderPTR);
	
	printf("\n Print vector in foreach: ");
	Vector_ForEach(vectorPtr, PrintItemInt, NULL);
	
	Vector_Destroy(&vectorPtr, DestroyInt );
	
	printf("\n\n---END---\n");
	
	return VECTOR_SUCCESS;
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

/*
Vector_t* SetUP(int _numOfItems)
{
	Vector_t* a_vec;
	size_t _initialSize = INIT_SIZE;
	size_t _extensionBblockSize = _initialSize / 2 ;
	Vector_Result result;
	int i;
	int new_data = 30;
	
	srand(time(NULL));
	
	a_vec = Vector_Create(_initialSize, _extensionBblockSize); 
	
	for (i=1; i<=_numOfItems; ++i)
	{
		if (RANDOM_NUMER_TEST) 
		{
			result = Vector_Append(a_vec, rand() % 1000 );
		}
		else
		{
			result = Vector_Append(a_vec, new_data = new_data *2 ); 
			new_data = new_data / 2.2;
		}
		
		if(VECTOR_SUCCESS != result) { printf("\nERROR %d\n", result); return NULL; }
		
	}
	
	printf("\n----------------------------\n new array vector is ready: \n");
	#if defined(PRINTARRAY) 
	VectorPrint(a_vec); 
	putchar('\n');
	#endif
	

	return a_vec;
}

*/


