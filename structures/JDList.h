/*
 * Header file for a generic doubly linked list.
 */

#ifndef JDLIST_H
#define JDLIST_H

#include <stddef.h>
#include "JDError_codes.h"

struct JDTwoWayNode {
	void* value;			/* Pointer to node data. */
	struct JDTwoWayNode* next;	/* Pointer to next node. */
	struct JDTwoWayNode* prev;	/* Pointer to prev node. */
};

struct JDList {
	struct JDTwoWayNode* head;	/* Pointer to the first node in list. */
	struct JDTwoWayNode* tail;	/* Pointer to the last node in list.  */
	int count;			/* The number of nodes in list.       */
	size_t elem_size;		/* The size of value being stored.    */
	void(*dtor)(void*);		/* Pointer to a value destructor.     */
};

/******************************************************************************/

/* Prepares a JDList for operations. */
int JDList_init(struct JDList* list, size_t elem_size, void(*dtor)(void*));

/* Destroys a list, if a destructor was passed to init, it will be called for every node in list. */
int JDList_dealloc(struct JDList* list);

/* Add an item to back of list. */
int JDList_append(struct JDList* list, void* item);

/* Add an item to front of list. */
int JDList_prepend(struct JDList* list, void* item);

/* Look at first item of list, does not remove it. */
int JDList_front(struct JDList* list, void* dest);

/* Look at last item of list, does not remove it. */
int JDList_back(struct JDList* list, void* dest);

/* Remove the first item in list. */
int JDList_remove(struct JDList* list, void* dest);

/* Remove the last item in list. */
int JDList_remove_back(struct JDList* list, void* dest);

/* Remove all nodes who satisfy predicate. */
int JDList_remove_if(struct JDList* list, int(*predicate)(void*));

/* Copy all values in nodes into an array. List maintains ownership. */
int JDList_array(struct JDList* list, void* dest);

/* Return the number of nodes in list. */
int JDList_count(struct JDList* list);

/* Test whether list is empty. */
int JDList_empty(struct JDList* list);

/* Returns true if key is found in list. */
int JDList_contains(struct JDList* list, void* key, int(*cmp)(void*,void*));

/* Inserts a value after the value key in list. */
int JDList_insert_after(struct JDList* list, void* key, void* value, int(*cmp)(void*,void*));

/* Removes a value after the value key in list. */
int JDList_remove_after(struct JDList* list, void* key, int(*cmp)(void*,void*));

/* Inserts a value before the value key in list. */
int JDList_insert_before(struct JDList* list, void* key, void* value, int(*cmp)(void*,void*));

/* Removes a value before the value key in list. */
int JDList_remove_before(struct JDList* list, void* key, int(*cmp)(void*,void*));

#endif
