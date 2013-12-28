/*
 * Implementation of a Queue using a singly linked list as its underlining data structure.
 */

#include <stdlib.h>
#include "JDQueue.h"

int JDQueue_init(struct JDQueue* q, size_t elem_size, void(*dtor)(void*))
{
	if (q == NULL) return JD_NULLPTR;
	
	q->list = malloc(sizeof(struct JDForwardList));
	if (q->list == NULL) return JD_FMALLOC;

	return JDForwardList_init(q->list, elem_size, dtor);
}

int JDQueue_dealloc(struct JDQueue* q)
{
	if (q == NULL) return JD_NULLPTR;
	if (q->list == NULL) return JD_NULLPTR;

	JDForwardList_dealloc(q->list);
	free(q->list);

	return JD_SUCCESS;
}

int JDQueue_enqueue(struct JDQueue* q, void* item)
{
	if (q == NULL) return JD_NULLPTR;
	return JDForwardList_append(q->list, item);
}

int JDQueue_dequeue(struct JDQueue* q, void* dest)
{
	if (q == NULL) return JD_NULLPTR;
	return JDForwardList_remove(q->list, dest);
}

int JDQueue_empty(struct JDQueue* q)
{
	if (q == NULL) return JD_NULLPTR;
	return JDForwardList_empty(q->list);
}

int JDQueue_count(struct JDQueue* q)
{
	if (q == NULL) return 0;
	return JDForwardList_count(q->list);
}

int JDQueue_array(struct JDQueue* q, void* dest)
{
	if (q == NULL) return JD_NULLPTR;
	return JDForwardList_array(q->list, dest);
}

int JDQueue_peek(struct JDQueue* q, void* dest)
{
	if (q == NULL) return JD_NULLPTR;
	return JDForwardList_front(q->list, dest);
}
