#include "apc.h"

//#define DEBUG

int main()
{
	/* Head points to Least Significant Digit node */
	/* Tail points to Most Significant Digit node */

	APC_t *head_num1 = NULL, *tail_num1 = NULL;
	APC_t *head_num2 = NULL, *tail_num2 = NULL;
	APC_t *head_sum  = NULL, *tail_sum  = NULL;
	APC_t *head_diff = NULL, *tail_diff = NULL;
	APC_t *head_prod = NULL, *tail_prod = NULL;
	APC_t *head_quo  = NULL, *tail_quo  = NULL;


	printf ("Program for only POSITIVE INTEGERS\n");	

	printf ("Enter Operand 1 : \n");
	func_scan_num (&head_num1, &tail_num1);
	print_list (tail_num1);

	printf ("Enter Operand 2 : \n");
	func_scan_num (&head_num2, &tail_num2);
	print_list (tail_num2);


	putchar ('\n');

	func_add (head_num1, head_num2, &head_sum, &tail_sum);
	printf ("Sum = \n");
	print_list (tail_sum);		// Tail is sent for printing as the tail points to MSD



	func_sub (head_num1, head_num2, &head_diff, &tail_diff);
	printf ("Difference = \n");
	print_list (tail_diff);		// Tail is sent for printing as the tail points to MSD

	
	func_mul (head_num1, head_num2, &head_prod, &tail_prod);
	printf ("Product = \n");
	print_list (tail_prod);		// Tail is sent for printing as the tail points to MSD


/*
//printf ("Division under progress\n");	
	func_div (head_num1, tail_num1, head_num2, tail_num2, &head_quo, &tail_quo);
	printf ("Quotient = \n");
	print_list (tail_quo);
	
*/
	return 0;
}


int func_div (APC_t *head_num1, APC_t *tail_num1, APC_t *head_num2, APC_t *tail_num2, APC_t **head_quo, APC_t **tail_quo)
{
	
	data_t quo, rem, data1, data2;

	APC_t *operand1  = tail_num1;	// Points to operand1
	APC_t *operand2  = tail_num2;	// Points to operand2

	APC_t *head_rem  = NULL;
	APC_t *tail_rem  = NULL;

	APC_t *head_prod = NULL;
	APC_t *tail_prod = NULL;

	APC_t *head_dvnd = NULL;
	APC_t *tail_dvnd = NULL;

	APC_t *head_mul  = NULL;
	APC_t *tail_mul  = NULL;

	int start_node, num_nodes, rem_nodes, num_nodes_num1, status;
	int idx, mul_fac;


	num_nodes_num1 = count_from_tail (tail_num1);
	print_list (tail_num1);
printf ("num_nodes_num1 = %d\n", num_nodes_num1);

	start_node = 1;
	rem_nodes  = 0;
	do
	{

	//	for (num_nodes = start_node; num_nodes <= num_nodes_num1; num_nodes++)
		for (num_nodes = 1; start_node + num_nodes - 1 <= num_nodes_num1; start_node++)
		{
			func_append_node (&head_dvnd, &tail_dvnd, tail_num1, start_node, num_nodes);

			printf ("Dvnd = \n");
			print_list (tail_dvnd);

			status = func_cmp_list (tail_dvnd, tail_num2);

			if (status == 1)
				break;
		}
		start_node += num_nodes;

		printf ("Divident > Divisor\n");
		print_list (tail_dvnd);

		mul_fac = 0;
		insert_at_first (&head_mul, &tail_mul, mul_fac);

		for (mul_fac = 2; mul_fac < 99; mul_fac++)
		{
			tail_mul -> data = mul_fac;
			func_mul (head_num2, head_mul, &head_prod, &tail_prod);

			printf ("prod = ");
			print_list (tail_prod);

			status = func_cmp_list (tail_dvnd, tail_prod);

			printf ("Satus = %d\n", status);

			if (status == -1)
			{
				delete_list (&head_prod, &tail_prod);
				break;
			}
			else
			{
				delete_list (&head_prod, &tail_prod);
			}

		}

		mul_fac--;
		tail_mul -> data = mul_fac;

		func_mul (head_num2, head_mul, &head_prod, &tail_prod);

		printf ("Quotient = %d\n", mul_fac);
		insert_at_first (head_quo, tail_quo, mul_fac);

		// Remainder
		func_sub (head_dvnd, head_prod, &head_rem, &tail_rem);
		print_list (tail_rem);

		//delete_list (&head_dvnd, &tail_dvnd);

		{
			rem_nodes = count_from_tail (tail_rem);

			func_copy_nodes (&head_dvnd, &tail_dvnd, tail_rem, rem_nodes); 

			printf ("Remainder = ");
			print_list (tail_rem);

		}
		sleep(1);

	} while (start_node <= num_nodes_num1);

	return SUCCESS;
}


int func_cmp_list (APC_t *tail_list1, APC_t *tail_list2)
{
	if (count_from_tail (tail_list1) < count_from_tail (tail_list2) )
	{
		return -1;
	}
 
	else if (count_from_tail (tail_list1) > count_from_tail (tail_list2) )
	{
		return 1;
	}

	else	// IF both equal
	{
		APC_t *operand1 = tail_list1;
		APC_t *operand2 = tail_list2;

		data_t data1, data2;

		while (operand1 || operand2)
		{
			data1 = operand1 -> data;
			data2 = operand2 -> data;

			if (data1 < data2)
			{
				return -1;
			}
			else if (data1 > data2)
			{
				return 1;
			}
			else
			{
				operand1 = operand1 -> prev;
				operand2 = operand2 -> prev;
			}
		}

		return 0;	// ie both equal
	}

} 


int func_append_node (APC_t **head_dst, APC_t **tail_dst, APC_t *tail_src, int start_src_node, int num_src_nodes)
{
	int idx;
	data_t data;
	APC_t *temp_src = tail_src;
	APC_t *temp_dst = *tail_dst;
	int count_src_nodes;

	count_src_nodes = count_from_tail (tail_src);

	if ((start_src_node > count_src_nodes) || (start_src_node + num_src_nodes - 1 > count_src_nodes))
	{
		return FAILURE;
	}

	// Traverse to the start_src_node
	for (idx = 1; idx < start_src_node; idx++)
		temp_src = temp_src -> prev;

	for (idx = 0; idx < num_src_nodes; idx++)
	{
		data     = temp_src -> data;

		insert_at_first (head_dst, tail_dst, data);

		temp_src = temp_src -> prev;
	}
	return SUCCESS;
}


int func_copy_nodes (APC_t **head_dst, APC_t **tail_dst, APC_t *tail_src, int start_node, int num_nodes)
{
	if (NULL != *head_dst)
	{
		delete_list (head_dst, tail_dst);
	}

	int idx;
	data_t data;
	APC_t *temp_src = tail_src;
	APC_t *temp_dst = *tail_dst;
	int count_src_nodes;

	count_src_nodes = count_from_tail (tail_src);

	if ((start_node > count_src_nodes) || (start_node + num_nodes > count_src_nodes))
	{
		return FAILURE;
	}

	// Traverse to the start_node
	for (idx = 1; idx < start_node; idx)
		temp_src = temp_src -> prev;

	for (idx = 1; idx <= num_nodes; idx++)
	{
		data     = temp_src -> data;

		insert_at_first (head_dst, tail_dst, data);

		temp_src = temp_src -> prev;
	}
	return SUCCESS;
}


/**********************************************************************************************/


int func_mul (APC_t *head_num1, APC_t *head_num2, APC_t **head_prod, APC_t **tail_prod)
{

#ifdef DEBUG	
	printf ("Inside func_mul\n");	
#endif

	data_t prod, carry, data1, data2;

	APC_t *operand1  = head_num1;	// Points to operand1
	APC_t *operand2  = head_num2;	// Points to operand2

	APC_t *curr_prod = NULL;	// Points to resultant node where current result has to be added; Modified after every cycle of multiplication
	APC_t *curr_LSD  = NULL;	// Points to resultant node in a particular cycle; Modified after each multiplication 

	int shift_prod = 0, idx;

	while (operand2)
	{
		data2 = (NULL == operand2) ? 0 : (operand2 -> data);
	
		operand1  = head_num1;
		curr_prod = *head_prod;

		for (idx = 0; idx < shift_prod; idx++)	// After each cycle of multiplication, the product is shifted left
			curr_prod = curr_prod -> next;

		curr_LSD = curr_prod;

		while (operand1)
		{

			data1 = (NULL == operand1) ? 0 : (operand1 -> data);
			
			prod  = data2 * data1 ;			// carry is from previous operation
			carry = prod / 100;							// New carry
			prod  = prod % 100;

			curr_prod = curr_LSD;

			add_prod (head_prod, tail_prod, &curr_prod, prod);
			if (*head_prod == *tail_prod)	// When the resultant list is empty
				curr_LSD = *head_prod;

			if(carry)
			{
				add_prod (head_prod, tail_prod, &curr_prod, carry);
			}

			operand1 = (NULL == operand1) ? NULL : (operand1 -> next);
	
			if (curr_LSD)
				curr_LSD = curr_LSD -> next;	
		}

#ifdef DEBUG
		printf ("Current product : ");
		print_list (*tail_prod);
#endif
		shift_prod++;
		operand2  = (NULL == operand2) ? NULL : (operand2 -> next);
	}

	return SUCCESS;
}


int func_sub (APC_t *head_num1, APC_t *head_num2, APC_t **head_diff, APC_t **tail_diff)
{

#ifdef DEBUG	
	printf ("Inside func_sub\n");
#endif


	/* Head points to LSB; And tail is towards head->next side (NOT prev side) */
	data_t diff, carry, data1, data2;
	APC_t *operand1 = head_num1;
	APC_t *operand2 = head_num2;
	APC_t *temp     = NULL;

	carry = 0;

	while (operand1 || operand2)
	{
		data1 = (NULL == operand1) ? 0 : (operand1 -> data);
		data2 = (NULL == operand2) ? 0 : (operand2 -> data);
/*
		if (operand2 == head_num2)		// LSD
			data2 = 100 - data2;
		else
			data2 = (99 - data2);		// 100's complement 
*/

		data2 = operand2 == head_num2 ? 100 - data2 : 99 - data2;

		diff  = data1 + data2 + carry;

		carry = diff > 99 ? diff %= 100, 1 : 0;

		insert_at_last (head_diff, tail_diff, diff);

		operand1 = (NULL == operand1) ? NULL : (operand1 -> next);
		operand2 = (NULL == operand2) ? NULL : (operand2 -> next);

	}


	/* If final carry is generated at MSD */
	if (carry == 0)	// ie if final carry is 1, then take 100's complement and assign -ve sign
	{
		(*head_diff) -> data = 100 - ((*head_diff) -> data);

		temp = (*head_diff) -> next;		// next higher 2 digits

		while (temp)
		{
			temp -> data = 99 - (temp -> data);

			temp = temp -> next;
		}
	}

	while (0 == (*tail_diff) -> data)		// To remove leading zeros
	{
		temp = *tail_diff;

		(*tail_diff) = (*tail_diff) -> prev;

		free (temp);
	}

	if (0 == carry)			// -ve number in 100's complement form
		(*tail_diff) -> data = -1 * ((*tail_diff) -> data);// Assigning sign to Most Significant Digit 


	return SUCCESS;
}



int func_add (APC_t *head_num1, APC_t *head_num2, APC_t **head_sum, APC_t **tail_sum)
{

#ifdef DEBUG	
	printf ("Inside func_add\n");
#endif

	data_t sum, carry, data1, data2;
	APC_t *operand1 = head_num1;
	APC_t *operand2 = head_num2;

	carry = 0;
	while (carry || operand1 || operand2)
	{
		data1 = (NULL == operand1) ? 0 : (operand1 -> data);
		data2 = (NULL == operand2) ? 0 : (operand2 -> data);
			
		sum   = data1 + data2 + carry;			// carry is from previous operation
		carry = sum / 100;						// New carry
		sum   = sum % 100;

		insert_at_last (head_sum, tail_sum, sum);

		operand1 = (NULL == operand1) ? NULL : (operand1 -> next);
		operand2 = (NULL == operand2) ? NULL : (operand2 -> next);
	}

	return SUCCESS;
}


int func_scan_num (APC_t **num, APC_t **tail)
{

	/* Scans and stores 2 digits per node */

#ifdef DEBUG	
	printf ("Inside func_scan_num\n");
#endif

	data_t node_data;
	char ch;
	char str_2digits[NODE_SIZE + 1] = {};

	int idx;

	__fpurge(stdin);
	do
	{
		for (idx = 0; idx < NODE_SIZE; idx++)
		{
			if((ch = getchar()) != '\n')
				str_2digits[idx] = ch;
			else
				break;
		}

		if (idx == 0)		// ie '\n' was received at even index
			break;

		str_2digits[idx] = '\0';

		node_data = atoi(str_2digits);
		insert_at_first(num, tail, node_data);
	} while (ch != '\n');

	if (idx == 1)// ie odd number of digits in the Least Significant Node, which implies entire number
		func_right_shift (*num, *tail);		// Perform one right shift of digits	
	
	__fpurge(stdin);

	return SUCCESS;

}

int func_right_shift (APC_t *head, APC_t *tail)
{

#ifdef DEBUG	
	printf ("Inside func_right_shift\n");
#endif

	data_t lower_digit, upper_digit;
	APC_t *temp = tail;

	upper_digit = 0;
	while (temp -> prev)
	{
		lower_digit  = (temp -> data) % 10;
		temp -> data = (upper_digit * 10) + (temp -> data / 10);
		upper_digit  = lower_digit;

		temp = temp -> prev;
	}

	temp -> data += (upper_digit * 10);

	return SUCCESS;
}
