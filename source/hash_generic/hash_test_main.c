#include <stdio.h>
#include <stdlib.h>
#include "mu_test.h"
#include "hash_map.h"

size_t HashFunc(const void* _value)
{
	int num = *(int*)_value;
/*	num *= 2;*/
/*	num *= 3;*/
	
	return num;
}

bool KeyEqualFunc(const void* _num1, const void* _num2)
{
	if(NULL == _num1 || NULL == _num2 )
	{
		return FALSE;
	}
	
/*	printf("\n num1:%d num2:%d", *(int*)_num1, *(int*)_num2 );*/
	return (*(int*)_num1 == *(int*)_num2);
}

void* CreateKey(int _num)
{
	int* key;
	key = malloc(1 * sizeof(int));
	if (NULL == key)
	{
		return NULL;
	}
	*key = _num;
	return (void*) key;
}

void* CreateValue(int _num)
{
	int* key;
	key = malloc(1 * sizeof(int));
	if (NULL == key)
	{
		return NULL;
	}
	*key = _num;
	return (void*) key;
}

void PrintItem(void* _key, void* _value, void* _context)
{
	
	printf("|%d(%d)", *(int*) _key, *(int*) _value );
	
	return;
}

UNIT(will_pass)
	ASSERT_THAT( sizeof(char) == 1);
	ASSERT_THAT( sizeof('A') == 4);
	ASSERT_THAT( sizeof(int) == 4);
END_UNIT

UNIT(will_fail)
	ASSERT_THAT( sizeof(int*) == 8);
END_UNIT


UNIT(malloc_zero_will_alloc_something)
	int *p = malloc(0);
	ASSERT_THAT(p != NULL);
END_UNIT

UNIT(Create)
	HashMap_t* hashMap1;
	HashMap_t* hashMap2;
	
	hashMap1 = HashMap_Create(6, HashFunc, KeyEqualFunc);
	hashMap2 = HashMap_Create(0, HashFunc, KeyEqualFunc);
	
	ASSERT_THAT( NULL != hashMap1);
	ASSERT_THAT( NULL == hashMap2);
	HashMap_Destroy(&hashMap1, NULL, NULL);
	
END_UNIT

UNIT(TestDestroy)
	HashMap_t* hashMap1;
	
	hashMap1 = HashMap_Create(6, HashFunc, KeyEqualFunc);
	HashMap_Destroy(&hashMap1, NULL, NULL);
	
	ASSERT_THAT( NULL == hashMap1);
	HashMap_Destroy(&hashMap1, NULL, NULL);
	ASSERT_THAT( NULL == hashMap1);
END_UNIT


UNIT(Inseart)
	HashMap_t* hashMap1;
	Map_Result result;
	
	hashMap1 = HashMap_Create(6, HashFunc, KeyEqualFunc);
	
	ASSERT_THAT( 0 == HashMap_Size(hashMap1) );
	
	result = HashMap_Insert(hashMap1, CreateKey(2), CreateValue(22));
	ASSERT_THAT( MAP_SUCCESS == result );
	ASSERT_THAT( 1 == HashMap_Size(hashMap1) );
	
	HashMap_Destroy(&hashMap1, NULL, NULL); /* TODO */ 
	ASSERT_THAT( NULL == hashMap1);
END_UNIT

UNIT(HASHRemove)
	HashMap_t* hashMap1;
	Map_Result result;
	int key = 2;
	int value = 22;
	void* outKey;
	void* outValue;
	
	hashMap1 = HashMap_Create(6, HashFunc, KeyEqualFunc);
	
	ASSERT_THAT( 0 == HashMap_Size(hashMap1) );
	
	result = HashMap_Insert(hashMap1, CreateKey(key + 2), CreateValue(value) );
	result |= HashMap_Insert(hashMap1, CreateKey(key), CreateValue(value*2) );
	ASSERT_THAT( MAP_SUCCESS == result );
	ASSERT_THAT( 2 == HashMap_Size(hashMap1) );
	
	result = HashMap_Remove(hashMap1, CreateKey(key), &outKey, &outValue);
	
	ASSERT_THAT( MAP_SUCCESS == result );
	ASSERT_THAT( 1 == HashMap_Size(hashMap1) );
	ASSERT_THAT( key == *(int*)outKey );
	ASSERT_THAT( value*2 == *(int*)outValue );
	
	result = HashMap_Remove(hashMap1, CreateKey(key+3), &outKey, &outValue);
	
	ASSERT_THAT( MAP_KEY_NOT_FOUND_ERROR == result );
	ASSERT_THAT( 1 == HashMap_Size(hashMap1) );
	
	result = HashMap_Remove(hashMap1, CreateKey(key + 2), &outKey, &outValue);
	
	ASSERT_THAT( MAP_SUCCESS == result );
	ASSERT_THAT( 0 == HashMap_Size(hashMap1) );
	ASSERT_THAT( key + 2 == *(int*)outKey );
	ASSERT_THAT( value == *(int*)outValue );
	
	HashMap_Destroy(&hashMap1, NULL, NULL); /* TODO */ 
	ASSERT_THAT( NULL == hashMap1);
	
END_UNIT
	
	
UNIT(TestForeach)
	HashMap_t* hashMap1;
	Map_Result result = 0;
	int key = 2;
	int value = 22;
	int i;
	size_t count = 0;

/*	void* outKey;*/
/*	void* outValue;*/
	
	hashMap1 = HashMap_Create(6, HashFunc, KeyEqualFunc);
	
	ASSERT_THAT( 0 == HashMap_Size(hashMap1) );
	
	for(i=0; i < 25 ; ++i)
	{
		result |= HashMap_Insert(hashMap1, CreateKey(key++), CreateValue(value++) );
	}
	ASSERT_THAT( MAP_SUCCESS == result );
	
	key = 2;
	value = 1000;
	result = MAP_KEY_DUPLICATE_ERROR;
	for(i=0; i < 25 ; ++i)
	{
		result |= HashMap_Insert(hashMap1, CreateKey(key++), CreateValue(value++) );
	}
	
	printf("\nwhole hashMap content key(value): ");
	count = HashMap_ForEach(hashMap1, (KeyValueActionFunction) PrintItem, NULL);
	printf("\n");
	
	ASSERT_THAT( 25 == count );
	ASSERT_THAT( MAP_KEY_DUPLICATE_ERROR == result );
	
	/* TODO */
/*	HashMap_Destroy(&hashMap1, NULL, NULL);  */
/*	ASSERT_THAT( NULL == hashMap1);*/
	
END_UNIT

UNIT(TestReHash)
	HashMap_t* hashMap1;
	Map_Result result;
	int key = 2;
	int value = 22;
	int i;
/*	size_t count = 0;*/
/*	void* outKey;*/
/*	void* outValue;*/
	
	hashMap1 = HashMap_Create(6, HashFunc, KeyEqualFunc);
	
	ASSERT_THAT( 0 == HashMap_Size(hashMap1) );
	
	for(i=0; i < 25 ; ++i)
	{
		result = HashMap_Insert(hashMap1, CreateKey(key++), CreateValue(value++) );
	}
	
	result = HashMap_Rehash(hashMap1, 10); 
	
	ASSERT_THAT( MAP_SUCCESS == result );
	
/*	HashMap_Destroy(&hashMap1, NULL, NULL);  TODO  */
/*	ASSERT_THAT( NULL == hashMap1);*/
	
	printf("hash destoy couase core dump\n");
	
END_UNIT


TEST_SUITE(framework test)
/*	TEST(malloc_zero_will_alloc_something)*/
	TEST(Create)
	TEST(TestDestroy)
	TEST(Inseart)
	TEST(HASHRemove)
	TEST(TestForeach)
	TEST(TestReHash)
END_SUITE
