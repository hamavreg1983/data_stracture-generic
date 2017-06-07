#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <stdint.h>



/*	This H file contines the defantions and macros Yuval uses in its projects
//
//	Yuval Hamberg April 2017
//	yuval.hamberg@gmail.com
 */

/* ~~~ macro ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define RETURN_IF_EQUAL(check, checkAgainst, returnVal) if((checkAgainst) == (check)) \
															{ \
															return (returnVal); \
															}
#define RETURN_VOID_IF_EQUAL(check, checkAgainst) if((checkAgainst) == (check)) \
															{ \
															return; \
															}

#define RELEASE_MEMORY_AND_RETURN_IF_EQUAL(check, checkAgainst, toFree, returnVal) if((checkAgainst) == (check)) \
																		{\
																			free(toFree); \
																			return (returnVal); \
																		}





#define IS_STRUCT_ILLEGAL_PREFORM(struct, returnVal) 	if(NULL == (struct) || ALIVE_MAGIC_NUMBER != struct->m_magicNumber)\
														{\
														warn("Input illegal (%p). file:%s at line %d.", ((void*)struct),__FILE__, __LINE__);\
														returnVal\
														}

#define WARN_IF_EQUAL(check, checkAgainst) if((checkAgainst) == (check)) \
															{ \
															warn("At file %s:%d. error send. result %d.", __FILE__, __LINE__, (check) ); \
															}

/* ~~~ defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef TRUE

#define TRUE 1
#define FALSE 0

#endif /* TRUE */

typedef int bool;
typedef unsigned int Uint;
typedef unsigned int Uint_t;
typedef uint64_t LUint;





/* ~~~ misc ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* color coded, for color printing debug */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"



#endif /* __DEFINES_H__ */
