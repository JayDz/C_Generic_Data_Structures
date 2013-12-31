#include <stdlib.h>

#include "JDDeque.h"

#define JD_NULL_CHECK(x,y) if ((x) == NULL) return JD_NULLPTR; if ((y) == NULL) return JD_NULLPTR_ARG

int JDDeque_init(struct JDDeque* d, size_t elem_size, void(*dtor)(void*))
{
	if (d == NULL) return JD_NULLPTR;
	d->list = malloc(sizeof(struct JDList));
	if (d->list == NULL) return JD_FMALLOC;

	return JDList_init(d->list, elem_size, dtor);
}

int JDDeque_dealloc(struct JDDeque* d)
{
	if (d == NULL) return JD_NULLPTR;
	JDList_dealloc(d->list);
	free(d->list);

	return JD_SUCCESS;
}

int JDDeque_append(struct JDDeque* d, void* item)
{
	JD_NULL_CHECK(d,item);

	return JDList_append(d->list, item);
}

int JDDeque_prepend(struct JDDeque* d, void* item)
{
	JD_NULL_CHECK(d,item);

	return JDList_prepend(d->list, item);
}

int JDDeque_remove(struct JDDeque* d, void* dest)
{
	JD_NULL_CHECK(d,dest);

	return JDList_remove(d->list, dest);
}

int JDDeque_remove_back(struct JDDeque* d, void* dest)
{
	JD_NULL_CHECK(d,dest);

	return JDList_remove_back(d->list, dest);
}

int JDDeque_remove_if(struct JDDeque* d, int(*pred)(void*))
{
	JD_NULL_CHECK(d,pred);

	return JDList_remove_if(d->list, pred);
}

int JDDeque_front(struct JDDeque* d, void* dest)
{
	JD_NULL_CHECK(d,dest);

	return JDList_front(d->list, dest);
}

int JDDeque_back(struct JDDeque* d, void* dest)
{
	JD_NULL_CHECK(d,dest);

	return JDList_back(d->list, dest);
}

int JDDeque_array(struct JDDeque* d, void* dest)
{
	JD_NULL_CHECK(d,dest);

	return JDList_array(d->list, dest);
}

int JDDeque_count(struct JDDeque* d)
{
	if (d == NULL) return JD_NULLPTR;
	return JDList_count(d->list);
}

int JDDeque_empty(struct JDDeque* d)
{
	if (d == NULL) return TRUE;
	return JDList_empty(d->list);
}
