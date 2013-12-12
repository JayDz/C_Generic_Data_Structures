/*
 * A generic stack using an array of a initial size 8.
 *
 * Date: 12/01/2013
 */

#ifndef JDARRAYSTACK_H
#define JDARRAYSTACK_H

#include <stddef.h>

struct JDArrayStack {
	size_t array_length;            /* Keep a count of our array length for resizing purposes. */
	size_t count;                   /* Keep a count of the number of elements in stack.        */
	size_t elem_size;               /* We must know the size of the elements in our stack.     */ 
	void*  elem;                    /* A pointer to our dynamically allocated array.           */
	void(*dealloc_elements)(void*); /* A client provided destrutor for complex elements.       */
};

/*********************************************************************************/
/* Function prototypes for the creation and destruction of a JDStack. */

/* initialize a JDStack with an initial alloc size of 8 elements. */
/* Can optionally pass a function pointer for deallocation of a more complex type*/
void JDArrayStack_init(struct JDArrayStack* s, size_t element_size, void(*)(void*));

/* destroy a JDStack. */
void JDArrayStack_dealloc(struct JDArrayStack* s);

/*********************************************************************************/
/* Function prototypes for the usual operations we need to perform on a stack */

/* push a value onto our stack. */
void JDArrayStack_push(struct JDArrayStack* s, void* value);

/* pop a value from our stack and copy into a dest variable. */
void JDArrayStack_pop(struct JDArrayStack* s, void* dest);

/* returns TRUE(1) if our stack is empty or FALSE(0) if it's not. */
int JDArrayStack_empty(struct JDArrayStack* s);

/* returns the number of elements currently in our stack. */
size_t JDArrayStack_count(struct JDArrayStack* s);

/* stores values in stack into an array dest. dest should point to block large enough for all stack values */
void JDArrayStack_array(struct JDArrayStack* s, void* dest);

/* return value at the top of stack, without removing it. */
void JDArrayStack_peek(struct JDArrayStack* s, void* dest);

#endif
