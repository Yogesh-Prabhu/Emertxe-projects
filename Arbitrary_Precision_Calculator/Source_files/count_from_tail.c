#include "apc.h"

int count_from_tail (APC_t *tail)
{
	if (NULL == tail)
		return LIST_EMPTY;

	APC_t *ptr = tail;
	int count = 0;

	while (ptr)
	{
		count++;
		ptr = ptr -> prev;
	}

	ptr = NULL;

	return count;
}
