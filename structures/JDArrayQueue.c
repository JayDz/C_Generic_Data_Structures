#define NDEBUG
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "JDArrayQueue.h"

#define ARRAY_LENGTH 8

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* Copy elements in queue into an array dest. */
static void copy_elements(void* dest, struct JDArrayQueue* q)
{
	int i;
	for (i = 0; i < q->count; ++i) {
		int index = ((i + q->head) % q->count);
		void* elem_addr = (char*)q->elem + (index * q->elem_size);
		void* dest_addr = (char*)dest + (i * q->elem_size);
		memcpy(dest_addr, elem_addr, q->elem_size);
	}
}

/* Will increase the size of heap memory by nsize bytes. */
static void resize(struct JDArrayQueue* q, size_t nsize)
{
	void* temp = malloc(q->elem_size * nsize);
	assert(temp != NULL);

	copy_elements(temp, q);
	
	free(q->elem);
	q->elem = temp;
	q->head = 0;
	q->tail = q->count;
	q->array_length = nsize;
}

int JDArrayQueue_init(struct JDArrayQueue* q, size_t elem_size, void(*dctor)(void*))
{
	if (q == NULL) return JD_NULLPTR;

	q->elem = malloc(elem_size * ARRAY_LENGTH);
	if (q->elem == NULL) return JD_FMALLOC;

	q->head = 0;
	q->tail = 0;
	q->count = 0;
	q->elem_size = elem_size;
	q->array_length = ARRAY_LENGTH;
	q->dealloc_elements = dctor;

	return JD_SUCCESS;
}

int JDArrayQueue_dealloc(struct JDArrayQueue* q)
{
	if ((q == NULL) || (q->elem == NULL)) return JD_NULLPTR;
	
	if (q->dealloc_elements) {
		int i;
		for (i = 0; i < q->count; ++i) {
			int index = ((i + q->head) % q->count);
			void* elem_addr = (char*)q->elem + (index * q->elem_size);
			q->dealloc_elements(elem_addr);
		}

		q->dealloc_elements = NULL;
	}

	free(q->elem);
	q->elem = NULL;
	q->head = 0;
	q->tail = 0;
	q->count = 0;
	q->elem_size = 0;
	q->array_length = 0;

	return JD_SUCCESS;
}

int JDArrayQueue_enqueue(struct JDArrayQueue* q, void* value)
{
	if ((q == NULL) || (q->elem == NULL)) return JD_NULLPTR;
	if (value == NULL) return JD_NULLPTR_ARG;

	if (q->count == q->array_length) resize(q, q->array_length * 2);
	
	void* last_position = (char*)q->elem + (q->elem_size * q->tail);
	memcpy(last_position, value, q->elem_size);
	q->tail++;

	if (q->tail == q->array_length) q->tail = 0;
	q->count++;

	return JD_SUCCESS;
}

int JDArrayQueue_dequeue(struct JDArrayQueue* q, void* dest)
{
	if ((q == NULL) || (q->elem == NULL)) 	return JD_NULLPTR;
	if (!q->count) 				return JD_EMPTY;
	if (dest == NULL) 			return JD_NULLPTR_ARG;

	void* first_position = (char*)q->elem + (q->elem_size * q->head);
	memcpy(dest, first_position, q->elem_size);
	q->head++;
	q->count--;

	if (q->head == q->array_length) q->head = 0;

	return JD_SUCCESS;
}

int JDArrayQueue_empty(struct JDArrayQueue* q)
{
	if (q == NULL) return TRUE;
	if (q->count)  return FALSE;
	return TRUE;
}

size_t JDArrayQueue_count(struct JDArrayQueue* q)
{
	if ((q == NULL) || (q->elem == NULL)) return JD_NULLPTR;
	return q->count;
}

int JDArrayQueue_array(struct JDArrayQueue* q, void* dest)
{
	if ((q == NULL) || (q->elem == NULL)) return JD_NULLPTR;
	if (dest == NULL) return JD_NULLPTR_ARG;

	copy_elements(dest, q);

	return JD_SUCCESS;
}

int JDArrayQueue_peek(struct JDArrayQueue* q, void* dest)
{
	if ((q == NULL) || (q->elem == NULL)) 	return JD_NULLPTR;
	if (!q->count) 				return JD_EMPTY;
	if (dest == NULL) 			return JD_NULLPTR_ARG;

	void* first_position = (char*)q->elem + (q->elem_size * q->head);
	memcpy(dest, first_position, q->elem_size);

	return JD_SUCCESS;
}
