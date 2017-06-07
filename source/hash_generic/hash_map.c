#include <stdio.h>
#include <stddef.h>  /* size_t */
#include <stdlib.h> /* free */
#include <math.h> /* sqtr */
#include <string.h> /* memcpy */

#include "hash_map.h"
#include "list_itr.h"

#define ALIVE_MAGIC_NUMBER 0xbeefbeef
#define DEAD_MAGIC_NUMBER 0xdeadbeef

#define DELETED_VALUE -1
#define EMPTY_VALUE -2

#define NOT_FOUND -3

#define INCREASE_SIZE_FACTOR 1.0 /* increase the size of memory by this factor to improve efficent */

/*
//	This is implementation of hash map.
//
//	Yuval Hamberg March 2017
// 	yuval.hamberg@gmail.com
*/

typedef struct TreeNode TreeNode_t;

struct Hash
{
	int m_alive_magicNumber;		/* checking to see that this hash hasn't been destroyed */
	
	List_t** m_array;				/* pointer to array */
	size_t m_originalhashSize;		/* original size given by client */
	size_t m_hashSize;				/* actual size of array */
	size_t m_numOfItems;			/* number of occupied places in the table */
	size_t m_maxOfRehashOperations;	/* maximum amount of rehash opration which have been done over one inseart */
	size_t m_counterOfInsertions;	/* amount of insertion into the table */
	size_t m_allRehashOperations;	/* all rehash operation mutual counter */
	
	HashFunction m_hashFunc;		 /* store the pointer to hash function of the HashMap */
	EqualityFunction m_keysEqualFunc;/* store the pointer to function IsValueEqual */
	
};

typedef struct DataUnit {
	void* m_key;					
	void* m_value;
} DataUnit_t;

typedef struct keyNfunction{
	void* m_searchedKey;					
	EqualityFunction m_keysEqualFunc;
	
} keyNfunction;

typedef struct ActionNContext
{
	KeyValueActionFunction m_action;
	void* m_context;
} ActionNContext;

typedef struct TwoFunctions
{
	char bullShit[99]; /* TODO there is a memory leak. this will protect agisnst its harm. one day i would find the overflowe */
	SimpleFunc m_func1;
	SimpleFunc m_func2;
} TwoFunctions;



/* forward deaclartion */
/* *********************/


/*	Function recives a number does the hash calculating and return the calculated number.
*	recives a pointer to the hash (for hashing number) and the integer number to do the calculating on.
*	returns int number calculated.
*	error like ERR_NOT_INITIALIZED would return -1 TODO update */
static int Hashing(const Hash_t* _hash, const void* _key);


/*	Function finds the next prime number after given number.
*	recives an integer number to do the calculating on.
*	returns prime int number calculated.
*	no known errors */
static int PrimeNumberFinder(int _num);

/* Function recive a singal number and return TRUE if prime and FALSE if not */
int isPrime(int num);

/* verifiy heap pointer is not null and alive magic number. return TRUE if there is a problame */
static bool CheckHashParam(const HashMap *_map);


static DataUnit_t* CreateDataUnit(HashMap* _hash, const void* _key, const void* _value);

static bool IsItrAtEnd(const List* _list, ListItr _itr);

static Map_Result GetDataUnit(DataUnit_t* _dataUnit, void** _pKey, void** _pValue);

static bool IsKeyEqual(DataUnit_t* _dataUnit, keyNfunction* keyNfunctionUnit);

static ListItr KeyItemFoundInList(List* _list, void* _key, EqualityFunction _keysEqualFunc);

static Map_Result InsertSingaleDataUnit(HashMap* _hash, DataUnit_t* _dataUnit);

static bool AListActFunc(DataUnit_t* _item, ActionNContext* _actionNcontext);

static void TwoFuncFunc(DataUnit_t* _element, TwoFunctions* _functions);

/* Functions */
/* *********************/

/*Hash_t* HashCreate(const size_t _size)*/
HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	Hash_t* a_hash;
	List_t** array; /* chnaged to list** */
	int i, _new_size;
	
	if (0 == _capacity || NULL == _hashFunc || NULL == _keysEqualFunc)
	{
		return NULL;
	}
	
	/* increase size by INCREASE_SIZE_FACTOR and choose a prime number bigger than this */
	_new_size = _capacity * INCREASE_SIZE_FACTOR;
	_new_size = PrimeNumberFinder(_new_size);
	
	#if !defined(NDEBUG) /* DEBUG */
	printf("\n_capacity:%u. _new_size:%u.",_capacity,_new_size);
	#endif
	
	a_hash = (Hash_t*) calloc(1,sizeof(Hash_t));
	if (NULL == a_hash)
	{
		return NULL;
	}
	
	array = (List_t**) calloc(_new_size,sizeof(List_t**));
	if (NULL == array)
	{
		free(a_hash);
		return NULL;
	}
	
/*	Init all lists*/
	for (i=0; i < _new_size ; ++i)
	{
		array[i] = List_Create();
		if (NULL == array[i])
		{
			free(a_hash);
			free(array);
			return NULL;
		}
	}
	
	
	a_hash->m_array = array;
	a_hash->m_hashSize = _new_size;
	a_hash->m_originalhashSize = _capacity;
	a_hash->m_alive_magicNumber = ALIVE_MAGIC_NUMBER;
	
	a_hash->m_hashFunc = _hashFunc;
	a_hash->m_keysEqualFunc = _keysEqualFunc;
	
	
	/* 			doing calloc so no need to zero 
	a_hash->m_numOfItems = 0;
	a_hash->m_maxOfRehashOperations = 0;
	a_hash->m_counterOfInsertions = 0;
	a_hash->m_allRehashOperations = 0;
	*/
	
	return a_hash;
}

/**
 * @brief destroy hash map and set *_map to null
 * @param[in] _map : map to be destroyed
 * @param[optional] _keyDestroy : pointer to function to destroy keys
 * @param[optional] _valDestroy : pointer to function to destroy values 
 * @details optionally destroy all keys and values using user provided functions
 */
void HashMap_Destroy(HashMap** _hash, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
	TwoFunctions functions;
	Uint_t i;
	
/*	printf(" TwoFunctions functions.bullShit %p \n ", functions.bullShit);*/
	
	if (NULL == _hash || NULL == *_hash || CheckHashParam(*_hash))
	{
		return;
	}
/*	printf ("_keyDestroy:%p _valDestroy:%p\n", _keyDestroy, _valDestroy);*/
/*	memcpy(functions.bullShit, "0", sizeof(functions) ); */
	functions.m_func1 = _keyDestroy ;
	functions.m_func2 = _valDestroy ;
/*	printf ("functions.m_func1:%p functions.m_func2:%p\n", functions.m_func1, functions.m_func2);*/
	
	HashMap_ForEach(*_hash, (KeyValueActionFunction) TwoFuncFunc, &functions);
	
	for (i=0; i < HashMap_Size(*_hash) ; ++i)
	{
		List_Destroy( &((*_hash)->m_array[i]), NULL);
	}
	
	free( ((*_hash)->m_array) );
	
	/* free hash */
	(*_hash)->m_alive_magicNumber = DEAD_MAGIC_NUMBER;
	free( (*_hash) );
	*_hash = NULL;
		
	return;
	
}

static void TwoFuncFunc(DataUnit_t* _element, TwoFunctions* _functions)
{
/*	printf("HERE\n");*/
/*	printf("_functions->m_func1 %p\n", _functions->m_func1);*/
/*	printf("_functions->m_func2 %p\n", _functions->m_func2);*/
	
	if (NULL != _functions->m_func1)
	{
		(_functions->m_func1)(_element->m_key);
	}
	if (NULL != _functions->m_func2)
	{
		(_functions->m_func2)(_element->m_value);
	}
	free(_element);
	
	return;
}



/** 
 * @brief Insert a key-value pair into the hash map.
 * @param[in] _map - Hash map to insert to, must be initialized
 * @param[in] _key - key to serve as index 
 * @param[in] _value - the value to associate with the key 
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_DUPLICATE_ERROR	if key alread present in the map
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_ALLOCATION_ERROR on failer to allocate key-value pair
 * @retval  MAP_UNINITIALIZED_ERROR
 * 
 * @warning key must be unique and destinct
 */
Map_Result HashMap_Insert(HashMap* _hash, const void* _key, const void* _value)
{
	DataUnit_t* newDataUnit;
	Map_Result result;

/*	check params */
	if ( CheckHashParam(_hash) )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	if(NULL == _key)
	{
		return MAP_KEY_NULL_ERROR;
	}

	newDataUnit = CreateDataUnit(_hash, _key, _value);
	if(NULL == newDataUnit)
	{
		return MAP_ALLOCATION_ERROR;
	}
/*	printf("\n data unit key:%d. valus:%d.", *(int*)newDataUnit->m_key, *(int*)newDataUnit->m_value);*/
	
	result = InsertSingaleDataUnit(_hash, newDataUnit);
	
	return result;
}

static Map_Result InsertSingaleDataUnit(HashMap* _hash, DataUnit_t* _dataUnit)
{
	Uint_t hashedKey;
	void* pKey;
	void* pValue;
	Map_Result result;
	List* currntListPtr;
	
	result = GetDataUnit(_dataUnit, &pKey, &pValue);
	if (MAP_SUCCESS != result)
	{
		return result;
	}
	
	
	/*	preform hash on key*/
	hashedKey = Hashing(_hash, pKey);
	
	currntListPtr = (_hash->m_array[hashedKey]);
	
	if ( NULL != (KeyItemFoundInList(currntListPtr, pKey, _hash->m_keysEqualFunc) ) )
	{
		free(_dataUnit);
		return MAP_KEY_DUPLICATE_ERROR;
	}
	
	/*	inseart struct to list head*/
	List_PushHead(currntListPtr, (void*) _dataUnit);
	
/*	update number of items */
	++(_hash->m_numOfItems);
	
	return MAP_SUCCESS;
}

static ListItr KeyItemFoundInList(List* _list, void* _key, EqualityFunction _keysEqualFunc)
{
	ListItr begin = NULL;
	ListItr end = NULL;
	ListItr itr = NULL;
	keyNfunction keyNfunctionUnit;
	if ((0 == List_Size(_list) )) 
	{
		return NULL;
		/* empty list cant exsist key */
	}
	
	begin = ListItr_Begin(_list);
	end = ListItr_End(_list);

	keyNfunctionUnit.m_searchedKey = _key;
	keyNfunctionUnit.m_keysEqualFunc = _keysEqualFunc;
	
	itr = ListItr_FindFirst(begin, end, (PredicateFunction) IsKeyEqual, &keyNfunctionUnit);
	
	if (IsItrAtEnd(_list, itr) )
	{
		return NULL;
	}
	else
	{
		return itr;
	}
}

static DataUnit_t* CreateDataUnit(HashMap* _hash, const void* _key, const void* _value)
{
	DataUnit_t* newDataUnit;
	
	newDataUnit = (DataUnit_t*) calloc(1,sizeof(DataUnit_t));
	if (NULL == newDataUnit)
	{
		return NULL;
	}
	newDataUnit->m_key = (void*) _key;
	newDataUnit->m_value = (void*) _value;
	
	return newDataUnit;
}

/** 
 * @brief Remove a key-value pair from the hash map.
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _searchKey - key to to search for in the map
 * @param[out] _pKey - pointer to variable that will get the key stored in the map equaling _searchKey
 * @param[out] _pValue - pointer to variable that will get the value stored in the map corresponding to foind key
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR
 * 
 * @warning key must be unique and destinct
 */
Map_Result HashMap_Remove(HashMap* _hash, const void* _searchKey, void** _pKey, void** _pValue)
{

	Uint_t hashedKey;
	List_t* currntListPtr;
	ListItr foundItr;
	DataUnit_t* itemData;
/*	keyNfunction keyNfunctionUnit;*/
	Map_Result result;
	
/*	check params */
	if ( CheckHashParam(_hash) || NULL == _pValue )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	if(NULL == _searchKey || NULL == _pKey)
	{
		return MAP_KEY_NULL_ERROR;
	}
	
/*	is hash full check*/
	if (_hash->m_numOfItems <= 0)
	{
		return MAP_HASH_UNDER_FLOW;
	}
	
/*	preform hash on key*/
	hashedKey = Hashing(_hash, _searchKey);
	
	currntListPtr = (_hash->m_array[hashedKey]);
	
/*	List_Print(currntListPtr, TempPrintItem);*/
	
	if ((0 == List_Size(currntListPtr) ))
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	
	/* set information to be send as struck to function */
/*	keyNfunctionUnit.m_searchedKey = (void*) _searchKey;*/
/*	keyNfunctionUnit.m_keysEqualFunc = _hash->m_keysEqualFunc;*/
	/* TODO what am i doing with keyNfunctionUnit */
	
	foundItr = KeyItemFoundInList(currntListPtr, (void*) _searchKey, (_hash->m_keysEqualFunc) ) ;
	
	/* if returned itr is end, meaning key desnt exsist in list. */
	if ( NULL == foundItr )
	{ 	
		/* already exsist */
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	
	itemData = (DataUnit_t*) ListItr_Remove(foundItr);
	if (NULL == itemData)
	{
		return MAP_KEY_REMOVAL_ERROR;
	}
	
	result = GetDataUnit(itemData, _pKey, _pValue);
	RETURN_IF_EQUAL(!result, MAP_SUCCESS, result);
	
	free(itemData);
	
/*	update number of items */
	--(_hash->m_numOfItems);

	return MAP_SUCCESS;
}

static bool IsItrAtEnd(const List* _list, ListItr _itr)
{
	return ListItr_Equals(_itr, ListItr_End(_list) );
}

size_t HashMap_Size(const HashMap* _hash)
/*size_t HashNumOfItems(const Hash_t* _hash)*/
{
	if ( CheckHashParam(_hash) )
	{
		return 0;
	}
	
	return _hash->m_numOfItems;
}


/** 
 * This method is optional in the homework 
 * @brief Iterate over all key-value pairs in the map and call a function for each pair
 * The user provided KeyValueActionFunction will be called for each element.  
 * Iteration will stop if the called function returns a zero for a given pair
 * 
 * @param[in] _map - Hash map to iterate over.
 * @param[in] _action - User provided function pointer to be invoked for each element
 * @param[in] _context - User provided function pointer to be invoked for each element
 * @returns number of times the user functions was invoked
 */
size_t HashMap_ForEach(const HashMap* _hash, KeyValueActionFunction _action, void* _context)
{
	int i;
	size_t count = 0;
	List* currntListPtr;
	ActionNContext actionNcontext;
	
	if ( CheckHashParam(_hash) || NULL == _action )
	{
		return 0;
	}
	
	
	actionNcontext.m_action = _action;
	actionNcontext.m_context = _context;
	
/*	for loop on array*/
	for(i=0 ; i < _hash->m_hashSize ; ++i)
	{
		currntListPtr = _hash->m_array[i];
		count += ListItr_CountIf(ListItr_Begin(currntListPtr), ListItr_End(currntListPtr), (PredicateFunction) AListActFunc, &actionNcontext);
		
	}
	
	return count;
}

static bool AListActFunc(DataUnit_t* _item, ActionNContext* _actionNcontext)
{
/*	TwoFunctions* temp = _actionNcontext->m_context;*/
/*	TODO printf("temp->m_func1:%p \n", temp->m_func1);*/
	
	return ((_actionNcontext->m_action)(_item->m_key ,_item->m_value , _actionNcontext->m_context) );
}

Map_Result HashMap_Find(const HashMap* _map, const void* _searchKey, void** _ppValue)
{
	ListItr itr;
	Uint_t hashedKey;
	List_t* currntListPtr;
	void* itemData;
	void* pKey;
	void* pValue;
	
	hashedKey = Hashing(_map, _searchKey);
	
	currntListPtr = (_map->m_array[hashedKey]);
	
	itr =  KeyItemFoundInList(currntListPtr, (void*) _searchKey, _map->m_keysEqualFunc);
	RETURN_IF_EQUAL(itr, NULL, MAP_KEY_NOT_FOUND_ERROR)
	
	itemData = ListItr_Get(itr);
	
	GetDataUnit(itemData, &pKey, &pValue);
	
	*_ppValue = pValue;
	
	return MAP_SUCCESS;
}

static Map_Result GetDataUnit(DataUnit_t* _dataUnit, void** _pKey, void** _pValue)
{
	
	if (NULL == _dataUnit || NULL == _pKey || NULL == _pValue )
	{
		return MAP_NULL_ERROR;
	}
	
	*_pKey = _dataUnit->m_key;
	*_pValue = _dataUnit->m_value;

	return MAP_SUCCESS;
}

static bool IsKeyEqual(DataUnit_t* _dataUnit, keyNfunction* keyNfunctionUnit)
{
	void* pKey;
	void* pValue;
	
	GetDataUnit(_dataUnit, &pKey, &pValue);
	
	return( (keyNfunctionUnit->m_keysEqualFunc)(pKey , keyNfunctionUnit->m_searchedKey) );
	
}


size_t HashCapacity(const Hash_t* _hash)
{
	if (NULL == _hash || ALIVE_MAGIC_NUMBER != _hash->m_alive_magicNumber)
	{
		return 0;
	}
	
	return _hash->m_originalhashSize;
}

void HashMap_Print(const HashMap* _map, KeyValueActionFunction _action)
{
	HashMap_ForEach(_map, _action, NULL);
	return;
}

/*double HashAverageRehashes(const Hash_t* _hash)*/
/*{*/
/*	if (NULL == _hash || ALIVE_MAGIC_NUMBER != _hash->m_alive_magicNumber)*/
/*	{*/
/*		return -1;*/
/*	}*/
/*	*/
/*	#if !defined(NDEBUG) */
/*	printf("\n m_allRehashOperations:%d",_hash->m_allRehashOperations);*/
/*	printf("\n m_counterOfInsertions:%d",_hash->m_counterOfInsertions);*/
/*	#endif*/
/*	*/
	/* do special to prevent deiviation by zero */
/*	if (0 ==_hash->m_counterOfInsertions)*/
/*	{*/
/*		return 0;*/
/*	}*/
/*	*/
/*	return (_hash->m_allRehashOperations / _hash->m_counterOfInsertions);*/
/*}*/


/*size_t HashMaxReHash(const Hash_t* _hash)*/
/*{*/
/*	if (NULL == _hash || ALIVE_MAGIC_NUMBER != _hash->m_alive_magicNumber)*/
/*	{*/
/*		return 0;*/
/*	}*/
/*	*/
/*	return _hash->m_maxOfRehashOperations;*/
/*}*/

/*void HashPrint(const Hash_t* _hash)*/
/*{*/
/*	int i;*/
/*	*/
/*	if (NULL == _hash || ALIVE_MAGIC_NUMBER != _hash->m_alive_magicNumber)*/
/*	{*/
/*		return;*/
/*	}*/
/*	*/
/*		for(i=0; i < _hash->m_hashSize ;++i)*/
/*	{*/
/*		if(DELETED_VALUE == _hash->m_array[i] || EMPTY_VALUE == _hash->m_array[i])*/
/*		{*/
/*			printf("- ");*/
/*		}*/
/*		else*/
/*		{*/
/*			printf("%d ",(int) _hash->m_array[i] );*/
/*		}*/
/*	}*/
/*	return;*/
/*}*/


static int Hashing(const Hash_t* _hash, const void* _key)
{
	Uint_t hashedKey;
	hashedKey = (_hash->m_hashFunc)(_key);
	
	return (hashedKey % _hash->m_hashSize);
}



static int PrimeNumberFinder(int _num)
{
	/* check number is positive? */
	
	while (!isPrime(_num))
	{
		++_num;
	}
	
	return _num;
}

int isPrime(int num) /* Prime number check  */
{
	int i;
	
	for (i=2; sqrt(num) > i;++i) /*  not up to num/2 it is better todo up to sqtr(num */
	{	 
		if ((num%i)==0)  /* Number is revided by i without reminder so it is not prime */
		{
			return FALSE;
		}
	}
	return TRUE;
}

static bool CheckHashParam(const HashMap *_map)
{
	return ( NULL == _map || ALIVE_MAGIC_NUMBER != _map->m_alive_magicNumber );
	/* return TRUE if values are not correct */
}


/** 
 * @brief Adjust map capacity and rehash all key/value pairs
 * @param[in] _map - exisiting map
 * @param[in] _newCapacity - new capacity shall be rounded to nearest larger prime number.
 * @return MAP_SUCCESS or MAP_ALLOCATION_ERROR
 */
Map_Result HashMap_Rehash(HashMap *_map, size_t newCapacity)
{
	List_t** newArray; 
	List_t** oldArray;
	int i, _new_size;
	Uint_t oldSize = _map->m_hashSize;
	void* dataUnit;
	
	if ( CheckHashParam(_map) || 0 == newCapacity )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
/*	check if newCapacity is equal to currnt Capacity or size, is TRUE return*/
	if (newCapacity == oldSize)
	{
		return MAP_SUCCESS;
	}
	
	_new_size = newCapacity * INCREASE_SIZE_FACTOR;
	_new_size = PrimeNumberFinder(_new_size);
	
	newArray = (List_t**) calloc(_new_size,sizeof(List_t**));
	RETURN_IF_EQUAL(newArray, NULL, MAP_ALLOCATION_ERROR);
	
/*	Init all lists*/
	for (i=0; i < _new_size ; ++i)
	{
		newArray[i] = List_Create();
		if (NULL == newArray[i])
		{
			free(newArray);
			return MAP_ALLOCATION_ERROR;
		}
	}
	
	oldArray = _map->m_array;
	_map->m_array = newArray;
	_map->m_hashSize = _new_size;
	_map->m_originalhashSize = newCapacity;
	
	for (i=0; i < oldSize ; ++i)
	{
		while (0 != List_Size(oldArray[i])) 
		{
/*			printf("\n while of rehash. i:%d. oldSize:%d. List_Size:%d. ", i, oldSize,  List_Size(oldArray[i]));*/
			List_PopHead(oldArray[i], &dataUnit);
			InsertSingaleDataUnit(_map, dataUnit);
		}
		List_Destroy(&oldArray[i], NULL);
	}
	
	free(oldArray);
	
	return MAP_SUCCESS;
}






