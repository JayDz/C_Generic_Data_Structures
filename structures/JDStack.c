/* 
 * An implementation of a generic stack.
 */

#include <stdlib.h>
#include "JDStack.h"

int JDStack_init(struct JDStack* stack, size_t elem_size, void(*dtor)(void*))
{
	if (stack == NULL) return JD_NULLPTR;

	stack->list = malloc(sizeof(struct JDForwardList));
	if (stack->list == NULL) return JD_FMALLOC;

	return JDForwardList_init(stack->list, elem_size, dtor);
}

int JDStack_dealloc(struct JDStack* stack)
{
	if (stack == NULL) return JD_NULLPTR;
	if (stack->list == NULL) return JD_NULLPTR;

	JDForwardList_dealloc(stack->list);
	free(stack->list);

	return JD_SUCCESS;
}

int JDStack_push(struct JDStack* stack, void* item)
{
	if (stack == NULL) return JD_NULLPTR;
	return JDForwardList_prepend(stack->list, item);
}

int JDStack_pop(struct JDStack* stack, void* dest)
{
	if (stack == NULL) return JD_NULLPTR;
	return JDForwardList_remove(stack->list, dest);
}

int JDStack_empty(struct JDStack* stack)
{
	if (stack == NULL) return JD_NULLPTR;
	return JDForwardList_empty(stack->list);
}

int JDStack_count(struct JDStack* stack)
{
	if (stack == NULL) return 0;
	return JDForwardList_count(stack->list);
}

int JDStack_peek(struct JDStack* stack, void* dest)
{
	if (stack == NULL) return JD_NULLPTR;
	return JDForwardList_front(stack->list, dest);
}

int JDStack_array(struct JDStack* stack, void* dest)
{
	if (stack == NULL) return JD_NULLPTR;
	return JDForwardList_array(stack->list, dest);
}
