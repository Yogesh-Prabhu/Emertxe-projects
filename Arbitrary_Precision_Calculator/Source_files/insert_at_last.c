#include "apc.h"

int insert_at_last(APC_t **head, APC_t **tail, data_t data)
{
	APC_t *new = malloc(sizeof(APC_t));

	if(NULL == new)
	{
		return FAILURE; 
	}

	new -> prev = new -> next = NULL;
	new -> data = data;

	if(NULL == *head)				// Empty list
	{
		*head = *tail = new;
	}

	else							// Non-empty list
	{
		(*tail) -> next = new;
		new -> prev     = *tail;
		*tail           = new;
	}

	return SUCCESS;
}
