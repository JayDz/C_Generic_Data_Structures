/*
 * A generic implementation of a Queue implemented with an array.
 *
 * Date: 12/12/13
 */

#ifndef JDARRAYQUEUE_H
#define JDARRAYQUEUE_H

#include <stddef.h>

struct JDArrayQueue {
	size_t array_length;			/* Keep count of allocated space. */
	size_t count;				/* Number of elements in Queue.   */
	size_t elem_size;			/* The size of elements in Queue  */
	void*  elem;				/* Pointer to array in heap.      */
	void(*dealloc_elements)(void*);		/* Pointer to destructor function */
	int    head;				/* Index of the head of Queue     */
	int    tail;				/* Index to the end of the Queue  */
};

/* Error codes */
#define JD_FMALLOC	0x00	/* Call to malloc failed. 			*/
#define JD_NULLPTR	0x00	/* Pointer to JDArrayQueue struct is NULL 	*/
#define JD_NULLPTR_ARG	0x01	/* Argument to function is NULL 		*/
#define JD_EMPTY	0x02	/* Queue is empty				*/
#define JD_SUCCESS	0x03	/* No errors detected				*/

/**********************************************************************************/
/* Function Prototypes */

/* Initializes a Queue. Elements in Queue are of size elem_size, des points to a function which can dealloc elements. */
int JDArrayQueue_init(struct JDArrayQueue* q, size_t elem_size, void(*dtor)(void*));

/* Destroys a Queue. */
int JDArrayQueue_dealloc(struct JDArrayQueue* q);

/* Add value to back of Queue. */
int JDArrayQueue_enqueue(struct JDArrayQueue* q, void* value);

/* Remove the element at the front of Queue. Store element value in address pointed to by dest. */
int JDArrayQueue_dequeue(struct JDArrayQueue* q, void* dest);

/* Returns TRUE(1) if Queue is empty, otherwise FALSE(0). */
int JDArrayQueue_empty(struct JDArrayQueue* q);

/* Returns the number of elements currently in Queue. */
size_t JDArrayQueue_count(struct JDArrayQueue* q);

/* Returns next element to be dequeue, but does not remove it. */
int JDArrayQueue_peek(struct JDArrayQueue* q, void* dest);

/* Stores elements in Queue into an array pointed to by dest. Assumes dest is large enough to hold all elements. */
int JDArrayQueue_array(struct JDArrayQueue* q, void* dest);

#endif
