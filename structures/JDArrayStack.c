#define NDEBUG
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "JDArrayStack.h"

#define ARRAY_LENGTH 8

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif 

/* Called when our array has no space for an additional element. */
/* Will increase the size of array by nsize. */
static void JDArrayStack_resize(struct JDArrayStack* s, size_t nsize);

void JDArrayStack_init(struct JDArrayStack* s, size_t element_size, void(*destructor)(void*))
{
	s->count = 0;
	s->elem_size = element_size;
	s->array_length = ARRAY_LENGTH;
	s->dealloc_elements = destructor; // An example for where this is useful: an aggregate type with dynamic memory.
	s->elem = malloc(ARRAY_LENGTH * element_size);
	
	assert(s->elem != NULL);
}

void JDArrayStack_dealloc(struct JDArrayStack* s)
{
	assert((s != NULL) && (s->elem != NULL));
	
	int i;	
	if (s->dealloc_elements != NULL) {
		for (i = 0; i < s->count; ++i) {
			void* elem_addr = (char*)s->elem + (s->elem_size * i);
			s->dealloc_elements(elem_addr);
		}
	}

	s->count = 0;
	s->elem_size = 0;
	s->array_length = 0;
	s->dealloc_elements = NULL;

	free(s->elem);
	s->elem = NULL;
}

void JDArrayStack_push(struct JDArrayStack* s, void* value)
{
	assert((s != NULL) && (s->elem != NULL));
	
	if (s->count == s->array_length) 
		JDArrayStack_resize(s, s->array_length * 2);
	
	void* top = (char*)s->elem + (s->elem_size * s->count);
	memcpy(top, value, s->elem_size);
	s->count++;
}

void JDArrayStack_pop(struct JDArrayStack* s, void* target)
{
	assert((s != NULL) && (s->elem != NULL));
	assert(s->count > 0);

	s->count--;
	void* top = (char*)s->elem + (s->elem_size * s->count);
	memcpy(target, top, s->elem_size);
}

int JDArrayStack_empty(struct JDArrayStack* s)
{
	assert((s != NULL) && (s->elem != NULL));

	if (s->count > 0) return FALSE;
	return TRUE;
}

int JDArrayStack_count(struct JDArrayStack* s)
{
	assert((s != NULL) && (s->elem != NULL));
	
	return s->count;
}

void JDArrayStack_array(struct JDArrayStack* s, void* dest)
{
	assert((s != NULL) && (s->elem != NULL));

	if (!s->count) return;

	int i;
	for (i = 0; i < s->count; ++i) {
		void* elem_addr = s->elem + (s->elem_size * i);
		memcpy(dest, elem_addr, s->elem_size);
		dest = (char*)dest + s->elem_size;
	}
}

static void JDArrayStack_resize(struct JDArrayStack* s, size_t nsize)
{
	s->elem = realloc(s->elem, nsize * s->elem_size);
	
	assert(s->elem != NULL);
	s->array_length = nsize;
}
