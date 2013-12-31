/*
 * A generic implementation of a Deque utilizing a doubly linked list.
 */

#ifndef JDDEQUE_H
#define JDDEQUE_H

#include <stddef.h>
#include "JDList.h"

struct JDDeque {
	struct JDList* list;
};

int JDDeque_init(struct JDDeque* d, size_t elem_size, void(*dtor)(void*));

int JDDeque_dealloc(struct JDDeque* d);

int JDDeque_append(struct JDDeque* d, void* item);

int JDDeque_prepend(struct JDDeque* d, void* item);

int JDDeque_remove(struct JDDeque* d, void* dest);

int JDDeque_remove_back(struct JDDeque* d, void* dest);

int JDDeque_remove_if(struct JDDeque* d, int(*predicate)(void*));

int JDDeque_front(struct JDDeque* d, void* dest);

int JDDeque_back(struct JDDeque* d, void* dest);

int JDDeque_array(struct JDDeque* d, void* dest);

int JDDeque_count(struct JDDeque* d);

int JDDeque_empty(struct JDDeque* d);

int JDDeque_contains(struct JDDeque* d, void* key, int(*cmp)(void*,void*));

#endif
