#include "apc.h"

int insert_at_first(APC_t **head, APC_t **tail, data_t data)
{
	APC_t *new = malloc (sizeof(APC_t));
	if(NULL == new)
	{
		return FAILURE;
	}

	new -> prev = new -> next = NULL;
	new -> data = data;

	if (NULL == *head)			// Empty list
		*head = *tail = new;

	else						// Non-Empty list
	{
		(*head) -> prev = new;
		new -> next     = *head;
		*head           = new;
	}

	return SUCCESS;
}
