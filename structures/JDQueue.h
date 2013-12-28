/*
 * An implementation of a generic Queue using a singly linked list as its underlining data structure.
 */

#ifndef JDQUEUE_H
#define JDQUEUE_H

#include <stddef.h>
#include "JDForwardList.h"

struct JDQueue {
	struct JDForwardList* list;
};

/* Prepares a JDQueue for usage. */
int JDQueue_init(struct JDQueue* q, size_t elem_size, void(*dtor)(void*));

/* Destroys a queue, if a destructor function was passed to init, it will be called for every element in queue. */
int JDQueue_dealloc(struct JDQueue* q);

/* Add an item to the back of the queue. */
int JDQueue_enqueue(struct JDQueue* q, void* item);

/* Remove an item from the front of the queue. */
int JDQueue_dequeue(struct JDQueue* q, void* dest);

/* Return the number of elements in queue. */
int JDQueue_count(struct JDQueue* q);

/* Returns true (non-zero) if queue is empty. Zero otherwise. */
int JDQueue_empty(struct JDQueue* q);

/* Stores item on front of queue into dest. Does not remove from queue. */
int JDQueue_peek(struct JDQueue* q, void* dest);

/* Stores all elements in queue into an array dest. */
int JDQueue_array(struct JDQueue* q, void* dest);

#endif
