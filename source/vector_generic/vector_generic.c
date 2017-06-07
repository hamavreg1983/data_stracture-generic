#include "vector_generic.h"
#include <stddef.h> /* size_t*/
#include <stdio.h> /* printf */
#include <stdlib.h>

#define ALIVE_NUM 0xbeefbeef
#define DEAD_NUM 0xdeadbeef

#define START_AT 1
#define SHRINK_VECTOR_WHEN_THIS_CHUNCK_BIGGER 2

/*	Generic type vector libary. builds and oprate void pointers data stratue of stack type.
//	
//	Yuval Hamberg Mar 2017
//	yuval.hamberg@gmail.com
 */


struct Vector{
	void** m_items;
	size_t m_initialCapacity;
	size_t m_size;
	size_t m_blockSize;
	size_t m_numberOfItems;
	int m_magicNumber;
};

Vector_t*	Vector_Create	(size_t _initialCapacity, size_t _blockSize)
{
	Vector_t* aVector = 0;
	void* array = 0;
	
	if (0 == _initialCapacity)
	{
		if (0 == _blockSize)
		{
			return NULL;
		}
		
		_initialCapacity = 1;
		/* size 0 is not safe option. if user want minimal space, i would creat the smalleset possible */
	}
	
	aVector = (Vector_t *) calloc(1, sizeof(Vector_t)); 
	if (NULL == aVector)
	{
		return NULL;
	}
	
	array = calloc(_initialCapacity, sizeof(void*)) ;
	if (NULL == array)
	{
		free(aVector); /* if failed we need to free the previus memory allocated */
		return NULL;
	}

	
	aVector->m_items = array;
	aVector->m_initialCapacity = _initialCapacity;
	aVector->m_size = _initialCapacity;
	aVector->m_numberOfItems = 0;
	aVector->m_blockSize = _blockSize;
	aVector->m_magicNumber = ALIVE_NUM; 
	

	return aVector;
}

void Vector_Destroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
	if (NULL == _vector || NULL == *_vector || ALIVE_NUM != (*_vector)->m_magicNumber)
	{
	return;
	}
	
	if (NULL != (*_vector)->m_items)
	{
		if (NULL != _elementDestroy)
		{
			/* loop and free elemnts */
			VectorElementAction casted_elementDestroy = (VectorElementAction) _elementDestroy;
			
			Vector_ForEach(*_vector, casted_elementDestroy, NULL);
			/* Should i test the return if i cant do anything if error? */
		}
		
		free((*_vector)->m_items);
		(*_vector)->m_magicNumber = DEAD_NUM;
	}
	
	free(*_vector);
	(*_vector) = NULL;  
	
	return;
}


Vector_Result Vector_Append	(Vector_t* _vector, void* _item)
{
	void* temp = NULL;
	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber || NULL == _item) /* check item is not null */
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (_vector->m_numberOfItems >= _vector->m_size) /* no more free space in stack */
	{
		if (0 >= _vector->m_blockSize)
		{
			return VECTOR_OVERFLOW;
		}
		temp = realloc(_vector->m_items, ((sizeof(void*) * (_vector->m_size + _vector->m_blockSize)) ) );
		if (NULL == temp)
		{
			return VECTOR_REALLOCATION_FAILED;
		}
		
		_vector->m_items = temp;
		
		_vector->m_size = _vector->m_size + _vector->m_blockSize;
	}
	
	_vector->m_items[_vector->m_numberOfItems] = _item; 
	
	++(_vector->m_numberOfItems);
	return VECTOR_SUCCESS;
}

Vector_Result Vector_Remove(Vector* _vector, void** _item)
{
	void* temp;
	
	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	
	if (_vector->m_numberOfItems < 1) 
	{ 
		/* vector is already empty */
		return VECTOR_UNDERFLOW;
	}
	
	--(_vector->m_numberOfItems);

	*_item = _vector->m_items[_vector->m_numberOfItems];
	_vector->m_items[_vector->m_numberOfItems] = NULL; 
	
	/* should resize now that its smaller? */
	if (_vector->m_numberOfItems + ((_vector->m_blockSize) * SHRINK_VECTOR_WHEN_THIS_CHUNCK_BIGGER) < _vector->m_size) 
	{
		temp = realloc(_vector->m_items, sizeof(void*) * (_vector->m_size - _vector->m_blockSize) );
		if (NULL == temp)
		{
			return VECTOR_REALLOCATION_FAILED;
		}
		_vector->m_items = temp;
		
		_vector->m_size = _vector->m_size - _vector->m_blockSize;
	}
	
	return VECTOR_SUCCESS;
}


Vector_Result	Vector_Get(const Vector_t* _vector, size_t _index, void** _pValue)
{
	_index -= START_AT;
	
	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber || NULL == _pValue) 
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if (_index >= _vector->m_numberOfItems || _index < 0)
	{
		return VECTOR_WRONG_INDEX;
	}
	
	*_pValue = _vector->m_items[_index];
	return VECTOR_SUCCESS;
}


Vector_Result Vector_Set(Vector* _vector, size_t _index, void**  _value)
{
	void* tempHolder;
	
	_index -= START_AT;
	
	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber || NULL == _value) 
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if (_index > _vector->m_numberOfItems || _index < 0)
	{
/*		printf("\n TEST index: %d _vector->m_numberOfItems: %d\n", _index, _vector->m_numberOfItems);*/
		return VECTOR_WRONG_INDEX;
	}
	
	/* return the item that was in the index before we set the new value */ 
	tempHolder = _vector->m_items[_index];
	_vector->m_items[_index] = *_value;
	*_value = tempHolder;
	
	return VECTOR_SUCCESS;
}



/* returns the amount of item preformed on. should be the size of vector */
size_t	Vector_ForEach	(const Vector_t* _vector, VectorElementAction _action, void* _context)
{
	Uint_t i;
	void* elemnt = 0;

	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber)
	{
		/* this function error woudld be zero */
		return 0;
	}
	
	for (i = START_AT ; i < Vector_Size(_vector) + START_AT ; ++i)
	{
		Vector_Get(_vector,i, &elemnt);
		
		#if !defined(NDEBUG) 
/*		printf("\n forEach loop i: %d, *elemnt: %d,",i+ START_AT , *(int*)elemnt );*/
		#endif
		
		/* i can't test returned value becouse sometimes i cast function without return
		if (TRUE != _action(elemnt, i + START_AT, _context) )
		{
			break;
		}*/
		
		_action(elemnt, i, _context);
		
	}
	return i - 1;
}
	

size_t	Vector_Size	(const Vector_t* _vector)
{
	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber)
	{
		/* this function error woudld be zero */
		return 0;
	}
	
	return _vector->m_numberOfItems;
}
	
size_t	Vector_Capacity	(const Vector_t* _vector)
{
	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber)
	{
		/* this function error woudld be zero */
		return 0;
	}
	
	return _vector->m_size;
}



void VectorPrint(Vector_t *_vector, VectorElementAction _printItem )
{
	if (NULL == _vector || ALIVE_NUM != _vector->m_magicNumber || NULL == _printItem)	
	{
		return;
	}
	
	
	/* foreach function */
	Vector_ForEach	(_vector, _printItem, NULL);

	return;
}









