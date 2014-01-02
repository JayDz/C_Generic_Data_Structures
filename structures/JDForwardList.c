/*
 * An implementation of a generic singly linked list.
 * 12/20/13
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "JDForwardList.h"

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

int JDForwardList_init(struct JDForwardList* list, size_t elem_size, void(*dtor)(void*))
{
	if (list == NULL) return JD_NULLPTR;

	list->head = NULL;
	list->tail = NULL;
	list->dtor = dtor;
	list->count = 0;
	list->elem_size = elem_size;

	return JD_SUCCESS;
}

int JDForwardList_dealloc(struct JDForwardList* list)
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

int JDForwardList_prepend(struct JDForwardList* list, void* value)
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

int JDForwardList_append(struct JDForwardList* list, void* value)
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

int JDForwardList_remove(struct JDForwardList* list, void* dest)
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

int JDForwardList_empty(struct JDForwardList* list)
{
	if (list == NULL) return TRUE;
	if (list->count)
		return FALSE;
	return TRUE;
}

int JDForwardList_count(struct JDForwardList* list)
{
	if (list == NULL) return JD_NULLPTR;
	return list->count;
}

int JDForwardList_array(struct JDForwardList* list, void* dest)
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

int JDForwardList_front(struct JDForwardList* list, void* dest)
{
	if (list == NULL)   return JD_NULLPTR;
	if (dest == NULL)   return JD_NULLPTR_ARG;
	if (!(list->count)) return JD_EMPTY;

	memcpy(dest, list->head->value, list->elem_size);

	return JD_SUCCESS;
}

int JDForwardList_back(struct JDForwardList* list, void* dest)
{
	if (list == NULL)   return JD_NULLPTR;
	if (dest == NULL)   return JD_NULLPTR_ARG;
	if (!(list->count)) return JD_EMPTY;

	memcpy(dest, list->tail->value, list->elem_size);

	return JD_SUCCESS;
}

int JDForwardList_remove_if(struct JDForwardList* list, int(*predicate)(void*))
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

int JDForwardList_contains(struct JDForwardList* list, void* key, int(*cmp)(void*,void*))
{
	if (list == NULL) 	return FALSE;
	if (key == NULL)	return FALSE;
	if (!(list->count))	return FALSE;

	int i;
	struct JDNode* curr = list->head;
	for (i = 0; i < list->count; ++i) {
		if (cmp) {
			if (!cmp(curr->value, key)) return TRUE;
		} else {
			if (!memcmp(curr->value, key, list->elem_size)) return TRUE;
		}
	}

	return FALSE;
}

int JDForwardList_insert_after(struct JDForwardList* list, void* key, void* value, int(*cmp)(void*,void*))
{
	if (list == NULL) 	return JD_NULLPTR;
	if (key == NULL) 	return JD_NULLPTR_ARG;
	if (value == NULL) 	return JD_NULLPTR_ARG;
	if (cmp == NULL)	return JD_NULLPTR_ARG;

	struct JDNode* curr = list->head;
	int i;
	for (i = 0; i < list->count; ++i) {
		if (!cmp(curr->value, key)) {
			if (i == 0) {
				return JDForwardList_append(list, value);
			} else if (i == (list->count - 1)) {
				struct JDNode* n;
				int ret = make_node(&n, value, list->elem_size);
				if (ret == JD_FMALLOC) return ret;

				curr->next = n;
				list->tail = n;
				list->count++;
				return JD_SUCCESS;
			} else {
				struct JDNode* n;
				int ret = make_node(&n, value, list->elem_size);
				if (ret == JD_FMALLOC) return ret;
				n->next = curr->next;
				curr->next = n;
				list->count++;
				return JD_SUCCESS;
			}
		}
		curr = curr->next;
	}
	return JD_NOTFOUND;
}

int JDForwardList_remove_after(struct JDForwardList* list, void* key, int(*cmp)(void*,void*))
{
	if (list == NULL)	return JD_NULLPTR;
	if (key == NULL)	return JD_NULLPTR_ARG;
	if (cmp == NULL)	return JD_NULLPTR_ARG;

	struct JDNode* curr = list->head;
	int i;
	for (i = 0; i < list->count; ++i) {
		if (!cmp(curr->value, key)) {
			if (list->head == list->tail) {
				return JD_SUCCESS;
			} else if (curr == list->tail) {
				return JD_SUCCESS;
			} else if (i == (list->count - 2)) {
				if (list->dtor) 
					list->dtor(list->tail->value);
				free(list->tail->value);
				free(list->tail);
				list->tail = curr;
				list->count--;
				return JD_SUCCESS;
			} else {
				struct JDNode* tmp = curr->next;
				curr->next = tmp->next;
				if (list->dtor)
					list->dtor(tmp->value);
				free(tmp->value);
				free(tmp);
				list->count--;
				return JD_SUCCESS;
			}
		}
		curr = curr->next;
	}
	return JD_NOTFOUND;
}
