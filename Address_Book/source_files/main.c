/***********************************************************
Project Title : 

Description :

Technologies used :

Date of Completion : 

***********************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "add.h"

AddrsBookInfo adrsInfo[MAX_ENTRIES];
uint sno  = 0 ;		// Serial number

int main(void)
{

	int flag_to_repeat = 1;

	OperationType operation;

	do
	{

		operation = func_display_main_menu_and_get_operation_type();

		func_operation(operation, adrsInfo);	


	}while(1);

	return 0;
}
