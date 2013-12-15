/*
 * This header file contains the prototypes of well known search algorithms: linear search and binary search.
 *
 * Date: 12/15/2013
 */

#ifndef JDSEARCH_H
#define JDSEARCH_H
	
#include <stddef.h>

/* An implementaiton of the linear search algorithm. 
 * base: pointer to base of array
 * key:  pointer to key we're to search for
 * elem_size: size in bytes of the elements in array
 * length: length of the array base
 * compare: a function pointer to function which is used for comparing elements. If set to NULL, we use memcmp std function. */
int jd_lsearch(void* base, void* key, size_t elem_size, size_t length, int(*compare)(void*,void*));

/* An implementation of the binary search algorithm. Same argument order as jd_lsearch(). */
int jd_bsearch(void* base, void* key, size_t elem_size, size_t length, int(*compare)(void*,void*));

#endif
