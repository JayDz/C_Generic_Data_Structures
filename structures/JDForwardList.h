/*
* An implementation of a generic singly linked list.
* 12/17/13
*/

#ifndef JDFORWARDLIST_H
#define JDFORWARDLIST_H

#include <stddef.h>
#include "JDError_codes.h"

struct JDNode {
	struct JDNode* next;	/* Pointer to the next node on list, or NULL. */
	void*  value;		/* Pointer to value for corresponding node.   */
};

struct JDList {
	struct JDNode* head;	/* Pointer to the first Node in list.		*/
	struct JDNode* tail;	/* Pointer to the last Node in list.		*/
	size_t  count;		/* The number of Nodes in list.			*/
	size_t  elem_size;	/* The size of the value we store in a Node.	*/
	void(*dtor)(void*);	/* Ptr to function to destroy elements in list. */
};

/********************************************************************************/
/* Function Prototypes */

/* Initializes a JDList. */
int JDList_init(struct JDList* list, size_t elem_size, void(*dtor)(void*));

/* Destroy all elements in List. */
int JDList_dealloc(struct JDList* list);

/* Creates a new Node containing data in value. New Node becomes the head of list. */
int JDList_prepend(struct JDList* list, void* value);

/* Creates a new Node containing data in value. New Node becomes the tail of list. */
int JDList_append(struct JDList* list, void* value);

/* Removes the head node of list, and stores its value in dest pointer. */
int JDList_remove(struct JDList* list, void* dest);

/* Returns true if list is empty, false otherwise. */
int JDList_empty(struct JDList* list);

/* Returns the number of elements (nodes) in list. */
int JDList_count(struct JDList* list);

/* Stores all values in list in an array dest. */
int JDList_array(struct JDList* list, void* dest);

/* Stores the value in front of the list in dest. Does not remove from list. */
int JDList_front(struct JDList* list, void* dest);

/* Stores the value in back of the list in dest. Does not remove from list. */
int JDList_back(struct JDList* list, void* dest);

/* Removes all nodes satisfying the predicate. Returns non-zero for true. */
int JDList_remove_if(struct JDList* list, int(*predicate)(void*));

#endif