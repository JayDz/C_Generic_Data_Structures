/*
 * Implementation of a generic doubly linked list. 
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "JDList.h"

static int make_node(struct JDTwoWayNode** n, void* value, size_t elem_size)
{
	*n = malloc(sizeof(struct JDTwoWayNode));
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
		struct JDTwoWayNode* tmp = list->head;
		list->head = list->head->next;
		free(tmp->value);
		free(tmp);
	}

	list->head = NULL;
	list->tail = NULL;
	list->dtor = NULL;
	list->count = 0;
	list->elem_size = 0;

	return JD_SUCCESS;
}

int JDList_append(struct JDList* list, void* item)
{
	if (list == NULL) return JD_NULLPTR;
	if (item == NULL) return JD_NULLPTR_ARG;

	struct JDTwoWayNode* n;
	int ret = make_node(&n, item, list->elem_size);
	if (ret == JD_FMALLOC) return ret;

	n->next = NULL;
	n->prev = NULL;
	if (list->count) {
		n->prev = list->tail;
		list->tail->next = n;
		list->tail = n;
	} else {
		list->head = n;
		list->tail = n;
	}

	list->count++;

	return JD_SUCCESS;
}

int JDList_prepend(struct JDList* list, void* item)
{
	if (list == NULL) return JD_NULLPTR;
	if (item == NULL) return JD_NULLPTR_ARG;

	struct JDTwoWayNode* n;
	int ret = make_node(&n, item, list->elem_size);
	if (ret == JD_FMALLOC) return ret;

	n->next = NULL;
	n->prev = NULL;
	if (list->count) {
		list->head->prev = n;
		n->next = list->head;
		list->head = n;
	} else {
		list->head = n;
		list->tail = n;
	}

	list->count++;

	return JD_SUCCESS;
}

int JDList_front(struct JDList* list, void* dest)
{
	if (list == NULL) 	return JD_NULLPTR;
	if (dest == NULL) 	return JD_NULLPTR_ARG;
	if (!(list->count)) 	return JD_EMPTY;

	memcpy(dest, list->head->value, list->elem_size);

	return JD_SUCCESS;
}

int JDList_back(struct JDList* list, void* dest)
{
	if (list == NULL) 	return JD_NULLPTR;
	if (dest == NULL) 	return JD_NULLPTR_ARG;
	if (!(list->count)) 	return JD_EMPTY;

	memcpy(dest, list->tail->value, list->elem_size);

	return JD_SUCCESS;
}

int JDList_remove(struct JDList* list, void* dest)
{
	if (list == NULL) 	return JD_NULLPTR;
	if (dest == NULL) 	return JD_NULLPTR_ARG;
	if (!(list->count)) 	return JD_EMPTY;

	struct JDTwoWayNode* tmp = list->head;
	list->head = list->head->next;
	memcpy(dest, tmp->value, list->elem_size);
	free(tmp->value);
	free(tmp);

	list->count--;

	return JD_SUCCESS;
}

int JDList_remove_back(struct JDList* list, void* dest)
{
	if (list == NULL) 	return JD_NULLPTR;
	if (dest == NULL) 	return JD_NULLPTR_ARG;
	if (!(list->count)) 	return JD_EMPTY;

	memcpy(dest, list->tail->value, list->elem_size);
	struct JDTwoWayNode* tmp = list->tail->prev;
	free(list->tail->value);
	free(list->tail);
	list->tail = tmp;

	list->count--;

	return JD_SUCCESS;
}

int JDList_remove_if(struct JDList* list, int(*predicate)(void*))
{
	if (list == NULL) return JD_NULLPTR;
	if (predicate == NULL) return JD_NULLPTR_ARG;

	struct JDTwoWayNode* prev_head = NULL;
	struct JDTwoWayNode* curr_node = list->head;
	int matches = 0;

	int i;
	for (i = 0; i < list->count; ++i) {
		if (prev_head == NULL) 
			if (list->head != curr_node)
				prev_head = list->head;

		if (predicate(curr_node->value)) {
			if (list->head == curr_node) {
				list->head = list->head->next;
				list->head->prev = NULL;
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
				curr_node->prev = prev_head;
				if (list->dtor) list->dtor(curr_node->value);
				free(curr_node->value);
				free(curr_node);
				curr_node = prev_head->next;
			}

			matches++;
		} else {
			struct JDTwoWayNode* tmp = curr_node;
			curr_node = curr_node->next;
			prev_head = tmp;
		}
	}

	list->count -= matches;

	return JD_SUCCESS;
}

int JDList_array(struct JDList* list, void* dest)
{
	if (list == NULL) 	return JD_NULLPTR;
	if (dest == NULL) 	return JD_NULLPTR_ARG;
	if (!(list->count)) 	return JD_EMPTY;

	int i;
	struct JDTwoWayNode* curr = list->head;
	for (i = 0; i < list->count; ++i) {
		void* addr = (char*)dest + (list->elem_size * i);
		memcpy(addr, curr->value, list->elem_size);
		curr = curr->next;
	}

	return JD_SUCCESS;
}

int JDList_count(struct JDList* list)
{
	if (list == NULL) return JD_NULLPTR;
	return list->count;
}

int JDList_empty(struct JDList* list)
{
	if (list == NULL) return TRUE;
	if (list->count)
		return FALSE;
	return TRUE;
}

int JDList_contains(struct JDList* list, void* key, int(*cmp)(void*, void*))
{
	if (list == NULL) 	return FALSE;
	if (key  == NULL) 	return FALSE;
	if (!(list->count)) 	return FALSE;
	
	int i;
	struct JDTwoWayNode* curr = list->head;
	for (i = 0; i < list->count; ++i) {
		if (cmp) {
			if (!cmp(curr->value, key)) return TRUE;
		} else {
			if (!memcmp(curr->value, key, list->elem_size)) return TRUE;
		}
		curr = curr->next;
	}

	return FALSE;
}

int JDList_insert_after(struct JDList* list, void* key, void* value, int(*cmp)(void*,void*))
{
	if (list == NULL) 	return JD_NULLPTR;
	if (key == NULL) 	return JD_NULLPTR_ARG;
	if (value == NULL) 	return JD_NULLPTR_ARG;
	if (cmp == NULL)	return JD_NULLPTR_ARG;

	struct JDTwoWayNode* curr = list->head;
	int i;
	for (i = 0; i < list->count; ++i) {
		if (!cmp(curr->value, key)) {
			if (i == 0) {
				return JDList_append(list, value);
			} else if (i == (list->count - 1)) {
				struct JDTwoWayNode* n;
				int ret = make_node(&n, value, list->elem_size);
				if (ret == JD_FMALLOC) return ret;

				curr->next = n;
				n->prev = curr;
				list->tail = n;
				list->count++;
				return JD_SUCCESS;
			} else {
				struct JDTwoWayNode* n;
				int ret = make_node(&n, value, list->elem_size);
				if (ret == JD_FMALLOC) return ret;
				
				n->next = curr->next;
				curr->next->prev = n;
				n->prev = curr;
				curr->next = n;
				list->count++;
				return JD_SUCCESS;
			}
		}
		curr = curr->next;
	}

	return JD_NOTFOUND;
}

int JDList_remove_after(struct JDList* list, void* key, int(*cmp)(void*,void*))
{
	if (list == NULL)	return JD_NULLPTR;
	if (key == NULL)	return JD_NULLPTR_ARG;
	if (cmp == NULL)	return JD_NULLPTR_ARG;

	struct JDTwoWayNode* curr = list->head;
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
				struct JDTwoWayNode* tmp = curr->next;
				curr->next = tmp->next;
				tmp->next->prev = curr;
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

int JDList_insert_before(struct JDList* list, void* key, void* value, int(*cmp)(void*,void*))
{
	if (list == NULL) 	return JD_NULLPTR;
	if (key == NULL) 	return JD_NULLPTR_ARG;
	if (value == NULL) 	return JD_NULLPTR_ARG;
	if (cmp == NULL) 	return JD_NULLPTR_ARG;

	struct JDTwoWayNode* curr = list->head;
	int i;
	for (i = 0; i < list->count; ++i) {
		if (!cmp(curr->value, key)) {
			if (i == 0) {
				return JDList_prepend(list,value);
			} else {
				struct JDTwoWayNode* n;
				int ret = make_node(&n, value, list->elem_size);
				if (ret == JD_FMALLOC) return ret;
				n->prev = curr->prev;
				struct JDTwoWayNode* tmp = curr->prev;
				tmp->next = n;
				n->next = curr;
				curr->prev = n;
				list->count++;
				return JD_SUCCESS;
			}
		}
		curr = curr->next;
	}

	return JD_NOTFOUND;
}

int JDList_remove_before(struct JDList* list, void* key, int(*cmp)(void*,void*))
{
	if (list == NULL) 	return JD_NULLPTR;
	if (key == NULL) 	return JD_NULLPTR_ARG;
	if (cmp == NULL)	return JD_NULLPTR_ARG;

	struct JDTwoWayNode* curr = list->head;
	int i;
	for (i = 0; i < list->count; ++i) {
		if (!cmp(curr->value, key)) {
			if (i == 0) {
				return JD_SUCCESS;
			} else if (i == 1) {
				if (list->dtor)
					list->dtor(list->head->value);
				free(list->head->value);
				free(list->head);
				list->head = curr;
				list->count--;
				return JD_SUCCESS;
			} else {
				struct JDTwoWayNode* kill = curr->prev;
				curr->prev = kill->prev;
				struct JDTwoWayNode* tmp = kill->prev;
				tmp->next = curr;
				if (list->dtor)
					list->dtor(kill->value);
				free(kill->value);
				free(kill);
				list->count--;
				return JD_SUCCESS;
			}
		}
		curr = curr->next;
	}

	return JD_NOTFOUND;
}
