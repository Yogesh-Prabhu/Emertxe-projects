#include "apc.h"

int add_prod (APC_t **head, APC_t **tail, APC_t **curr, data_t data)
{
	data_t sum, carry;

	if (*curr == NULL)	// If no data currently present, then create new node
	{
		insert_at_last (head, tail, data);		// head points to LSD, tail to MSD
		*curr = NULL;
	}

	else				// If data already present, then add the new data to it
	{
		sum = data + (*curr) -> data;
		
		carry = sum / 100;
		sum   = sum % 100;

		(*curr) -> data = sum;

		if (carry)		// If carry is generated, add this carry to next higher significant node
		{
			APC_t *temp = *curr;	
			*curr = (*curr) -> next;
			add_prod (head, tail, curr, carry);
			*curr = temp;
			temp  = NULL;
		}
			
		*curr = (*curr) -> next ;
	}

	return SUCCESS;
}
