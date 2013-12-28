/*
 * An implementation of a Stack using a singly linked list as the underlining data structure.
 */

#ifndef JDSTACK_H
#define JDSTACK_H

#include <stddef.h>
#include "JDError_codes.h"
#include "JDForwardList.h"

struct JDStack {
	struct JDForwardList* list; /* Pointer to singly linked list. */
};

/* Initializes a JDStack for operation. Must specify size of elements and an optional destructor for said elements.  */
int JDStack_init(struct JDStack* stack, size_t elem_size, void(*dtor)(void*));

/* Destroys stack, if initialized with a destructor, the destructor will be called on every element. */
int JDStack_dealloc(struct JDStack* stack);

/* Insert an item to the top of the stack. */
int JDStack_push(struct JDStack* stack, void* item);

/* Remove current item at top of stack. */
int JDStack_pop(struct JDStack* stack, void* dest);

/* Test whether stack is empty or not. */
int JDstack_empty(struct JDStack* stack);

/* Return the number of elements currently on stack. */
int JDStack_count(struct JDStack* stack);

/* Store all elements in stack into an array large enough to hold all elements. */
int JDStack_array(struct JDStack* stack, void* dest);

/* Get the value currently on top of stack. */
int JDStack_peek(struct JDStack* stack, void* dest);

#endif
