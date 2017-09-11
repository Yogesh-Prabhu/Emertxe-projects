#include "apc.h"

int delete_list (APC_t **head, APC_t **tail)
{
	if (!*head)
	{
		return LIST_EMPTY;
	}

	APC_t *temp = *head, *prev = NULL;

	while (temp)
	{
		prev = temp;
		temp = temp -> next;
		free (prev);
	}

	*head = *tail = NULL;

	temp  = prev  = NULL;

	return SUCCESS;
}
