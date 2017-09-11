#include "apc.h"

int print_list(APC_t *tail)
{
	if (NULL == tail)
		return LIST_EMPTY;

	APC_t *ptr = tail;		// Traversing from tail towards head as the tail points to MSB

	while(ptr)
	{

		printf ("%2.2d", ptr -> data);

		ptr = ptr -> prev;
	}
	printf("\n\n");

	return SUCCESS;
}
