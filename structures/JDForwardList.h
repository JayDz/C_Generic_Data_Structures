/*
* An implementation of a generic singly linked list.
* 12/20/13
*/

#ifndef JDFORWARDLIST_H
#define JDFORWARDLIST_H

#include <stddef.h>
#include "JDError_codes.h"

struct JDNode {
	struct JDNode* next;	/* Pointer to the next node on list, or NULL. */
	void*  value;		/* Pointer to value for corresponding node.   */
};

struct JDForwardList {
	struct JDNode* head;	/* Pointer to the first Node in list.		*/
	struct JDNode* tail;	/* Pointer to the last Node in list.		*/
	size_t  count;		/* The number of Nodes in list.			*/
	size_t  elem_size;	/* The size of the value we store in a Node.	*/
	void(*dtor)(void*);	/* Ptr to function to destroy elements in list. */
};

/********************************************************************************/
/* Function Prototypes */

/* Initializes a JDList. */
int JDForwardList_init(struct JDForwardList* list, size_t elem_size, void(*dtor)(void*));

/* Destroy all elements in List. */
int JDForwardList_dealloc(struct JDForwardList* list);

/* Creates a new Node containing data in value. New Node becomes the head of list. */
int JDForwardList_prepend(struct JDForwardList* list, void* value);

/* Creates a new Node containing data in value. New Node becomes the tail of list. */
int JDForwardList_append(struct JDForwardList* list, void* value);

/* Removes the head node of list, and stores its value in dest pointer. */
int JDForwardList_remove(struct JDForwardList* list, void* dest);

/* Returns true if list is empty, false otherwise. */
int JDForwardList_empty(struct JDForwardList* list);

/* Returns the number of elements (nodes) in list. */
int JDForwardList_count(struct JDForwardList* list);

/* Stores all values in list in an array dest. */
int JDForwardList_array(struct JDForwardList* list, void* dest);

/* Stores the value in front of the list in dest. Does not remove from list. */
int JDForwardList_front(struct JDForwardList* list, void* dest);

/* Stores the value in back of the list in dest. Does not remove from list. */
int JDForwardList_back(struct JDForwardList* list, void* dest);

/* Removes all nodes satisfying the predicate. Returns non-zero for true. */
int JDForwardList_remove_if(struct JDForwardList* list, int(*predicate)(void*));

/* Returns true if key is in list, otherwise false. */
int JDForwardList_contains(struct JDForwardList* list, void* key, int(*cmp)(void*,void*));

/* Inserts value after the key value in list. */
int JDForwardList_insert_after(struct JDForwardList* list, void* key, void* value, int(*cmp)(void*,void*));

/* Removes value after the key value in list. */
int JDForwardList_remove_after(struct JDForwardList* list, void* key, int(*cmp)(void*,void*));

#endif
