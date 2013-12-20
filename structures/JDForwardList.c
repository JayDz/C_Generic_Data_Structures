/*
 * An implementation of a generic singly linked list.
 * 12/17/13
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "JDForwardList.h"
#include <stdio.h>
static int make_node(struct JDNode** n, void* value, size_t elem_size)
{
	*n = malloc(sizeof(struct JDNode));
	if (*n == NULL) return JD_FMALLOC;

	(*n)->value = malloc(elem_size);
	if ((*n)->value == NULL) {
		free(*n);
		return JD_FMALLOC;
	}

	memcpy((*n)->value, value, elem_size);
	return JD_SUCCESS;
}

int JDList_init(struct JDList* list, size_t elem_size, void(*dtor)(void*))
{
	if (list == NULL) return JD_NULLPTR;

	list->head = NULL;
	list->tail = NULL;
	list->dtor = dtor;
	list->count = 0;
	list->elem_size = elem_size;

	return JD_SUCCESS;
}

int JDList_dealloc(struct JDList* list)
{
	
	if (list == NULL) return JD_NULLPTR;
	
	int i;
	for (i = 0; i < list->count; ++i) {
		if (list->dtor)
			list->dtor(list->head->value);
		struct JDNode* temp = list->head;
		list->head = list->head->next;
		free(temp->value);
		free(temp);
	}

	list->head = NULL;
	list->tail = NULL;
	list->dtor = NULL;
	list->count = 0;
	list->elem_size = 0;

	return JD_SUCCESS;
}

int JDList_prepend(struct JDList* list, void* value)
{
	if (list == NULL)  return JD_NULLPTR;
	if (value == NULL) return JD_NULLPTR_ARG;

	struct JDNode* n = NULL;
	int ret = make_node(&n, value, list->elem_size);
	if (ret == JD_FMALLOC) return ret;

	if (list->count) {
		n->next = list->head;
		list->head = n;
	} else {
		n->next = NULL;
		list->head = n;
		list->tail = n;
	}

	list->count++;

	return JD_SUCCESS;
}

int JDList_append(struct JDList* list, void* value)
{
	if (list == NULL)  return JD_NULLPTR;
	if (value == NULL) return JD_NULLPTR_ARG;

	struct JDNode* n = NULL;
	int ret = make_node(&n, value, list->elem_size);
	if (ret == JD_FMALLOC) return ret;	
	
	n->next = NULL;
	if (list->count) {
		list->tail->next = n;
	} else {
		list->head = n;
	}
	list->tail = n;

	list->count++;

	return JD_SUCCESS;
}

int JDList_remove(struct JDList* list, void* dest)
{
	if (list == NULL)   return JD_NULLPTR;
	if (dest == NULL)   return JD_NULLPTR_ARG;
	if (!(list->count)) return JD_EMPTY;

	struct JDNode* next = list->head->next;
	memcpy(dest, list->head->value, list->elem_size);
	
	free(list->head->value);
	free(list->head);
	list->head = next;

	list->count--;

	return JD_SUCCESS;
}

int JDList_empty(struct JDList* list)
{
	if (list == NULL) return TRUE;
	if (list->count)
		return FALSE;
	return TRUE;
}

int JDList_count(struct JDList* list)
{
	if (list == NULL) return JD_NULLPTR;
	return list->count;
}

int JDList_array(struct JDList* list, void* dest)
{
	if (list == NULL)   return JD_NULLPTR;
	if (dest == NULL)   return JD_NULLPTR_ARG;
	if (!(list->count)) return JD_EMPTY;

	int i;
	struct JDNode* curr = list->head;
	for (i = 0; i < list->count; ++i) {
		void* addr = (char*)dest + (list->elem_size * i);
		memcpy(addr, curr->value, list->elem_size);
		curr = curr->next;
	}

	return JD_SUCCESS;
}

int JDList_front(struct JDList* list, void* dest)
{
	if (list == NULL)   return JD_NULLPTR;
	if (dest == NULL)   return JD_NULLPTR_ARG;
	if (!(list->count)) return JD_EMPTY;

	memcpy(dest, list->head->value, list->elem_size);

	return JD_SUCCESS;
}

int JDList_back(struct JDList* list, void* dest)
{
	if (list == NULL)   return JD_NULLPTR;
	if (dest == NULL)   return JD_NULLPTR_ARG;
	if (!(list->count)) return JD_EMPTY;

	memcpy(dest, list->tail->value, list->elem_size);

	return JD_SUCCESS;
}

int JDList_remove_if(struct JDList* list, int(*predicate)(void*))
{
	if (list == NULL) return JD_NULLPTR;
	if (predicate == NULL) return JD_NULLPTR_ARG;

	struct JDNode* prev_head = NULL;
	struct JDNode* curr_node = list->head;
	int matches = 0;
	
	int i;
	for (i = 0; i < list->count; ++i) {
		if (prev_head == NULL) 
			if (list->head != curr_node)
				prev_head = list->head;

		if (predicate(curr_node->value)) {
			if (list->head == curr_node) {
				list->head = list->head->next;
				if (list->dtor) list->dtor(curr_node->value);
				free(curr_node->value);
				free(curr_node);
				curr_node = list->head;
			} else if (i == ((list->count)-1)) {
				list->tail = prev_head;
				if (list->dtor) list->dtor(curr_node->value);
				free(curr_node->value);
				free(curr_node);
			} else {
				prev_head->next = curr_node->next;
				if (list->dtor) list->dtor(curr_node->value);
				free(curr_node->value);
				free(curr_node);
				curr_node = prev_head->next;
			}

			matches++;
		} else {
			struct JDNode* tmp = curr_node;
			curr_node = curr_node->next;
			prev_head = tmp;
		}
	}
	
	list->count -= matches;

	return JD_SUCCESS;
}
