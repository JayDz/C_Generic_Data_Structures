#include <stddef.h>
#include <string.h>
#include "JDSearch.h"

int jd_lsearch(void* base, void* key, size_t elem_size, size_t length, int(*elem_cmp)(void*,void*))
{
	int i;
	for (i = 0; i < length; ++i) {
		void* addr = (char*)base + (i * elem_size);
		if (elem_cmp) {
			if (!elem_cmp(addr, key)) return i;
		} else { 
			if (!memcmp(addr, key, elem_size)) return i;
		}
	}

	return -1;
}

int jd_bsearch(void* base, void* key, size_t elem_size, size_t length, int(*elem_cmp)(void*,void*))
{
	int low  = 0;
	int high = length - 1;
	int index;
	while (high >= low) {
		index = (high + low) / 2;
		void* addr = (char*)base + (elem_size * index);

		int ans;
		if (elem_cmp)
			ans = elem_cmp(addr, key);
		else
			ans = memcmp(addr, key, elem_size);

		if (ans > 0)      high  = index - 1;
		else if (ans < 0) low   = index + 1;
		else              return index;
	}

	return -1;
}
