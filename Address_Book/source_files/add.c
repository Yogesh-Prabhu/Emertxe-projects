#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

#include "add.h"
#include "types.h"
#include "std_def.h"

#define _GNU_SOURCE 		// for fcloseall()

FILE *fptr_dst;
extern uint sno;


/* Macro to edit a entry by which is searched on key age */
#define CASE_EDIT_BY_AGE 												\
{																		\
	int idx;															\
	int flag_found = 0;													\
	char new_name[SZ_NAME];												\
	char new_addr[SZ_NAME];												\
	char new_phone[SZ_PHONE];											\
	unsigned char new_age;												\
	char new_email[SZ_EMAIL];											\
																		\
	printf("Enter the search key as : age\n");							\
	scanf("%hhu", &age);												\
	for(idx = 0; idx < sno; idx++)										\
	{																	\
		if(age == adrsInfo[idx].age)									\
		{																\
			flag_found = 1;												\
																		\
			printf("age found at Entry number %d\n", idx + 1);			\
			func_display_details(idx, adrsInfo);						\
																		\
			EDIT_OPTIONS;												\
																		\
		}																\
	}																	\
																		\
	if(flag_found == 0)													\
	{																	\
		printf("age NOT found\n");										\
		return e_failure;												\
	}																	\
}


/* Macro to search all other string type data viz. name, address, phone, email */
#define CASE_SEARCH(search_type) 										\
{																		\
	int idx;															\
	int flag_found = 0;													\
																		\
	char name[SZ_NAME];													\
	char addr[SZ_ADDR];													\
	char phone[SZ_PHONE];												\
	unsigned char age;													\
	char email[SZ_EMAIL];												\
																		\
	uint serial_no_csv;													\
	char name_csv[SZ_NAME];												\
	char addr_csv[SZ_ADDR];												\
	char phone_csv[SZ_PHONE];											\
	unsigned char age_csv;												\
	char email_csv[SZ_EMAIL];											\
																		\
	__fpurge(stdin);													\
																		\
	switch(search_type)											\
	{																	\
		case e_name:													\
			printf("Enter name to be searched\n");					\
			scanf("%[^\n]", name); 									\
			break;														\
																		\
		case e_address:													\
			printf("Enter address to be searched\n");					\
			scanf("%[^\n]", addr); 									\
			break;														\
																		\
		case e_phone:													\
			printf("Enter phone to be searched\n");					\
			scanf("%[^\n]", phone); 									\
			break;													\
																		\
		case e_age:														\
			printf("Enter age to be searched\n");					\
			scanf("%hhu", &age);											\
			break;														\
																		\
		case e_email:													\
			printf("Enter email to be searched\n");					\
			scanf("%[^\n]", email); 									\
			break;														\
																		\
		default:														\
			printf("\nUnrecognized Column\n");							\
			break;														\
	}																	\
																		\
	__fpurge(stdin);													\
																		\
	fptr_dst = fopen(CSV_FILENAME, "r");								\
	/* Error handling	*/												\
	if(NULL == fptr_dst)												\
	{																	\
		printf("Error : fopen failed to open file %s\n", CSV_FILENAME);	\
		return e_failure;												\
	}																	\
																		\
																		\
	for(idx = 0; idx < sno; idx++) /* Searching within .csv file*/		\
	{																	\
		fscanf(fptr_dst, "%u,%[^,],%[^,],%[^,],%hhu,%[^,],\n", &serial_no_csv, name_csv, addr_csv, phone_csv, &age_csv, email_csv);	\
																					\
		switch(search_type)															\
		{																			\
			case e_name:															\
				if(strcmp(name_csv, name) == 0)								\
				{																	\
					printf("\nName found at Entry number %d\n", idx + 1);				\
					func_display_details(idx, adrsInfo);						\
					flag_found = 1;												\
				}																	\
																					\
				break;																\
																					\
			case e_address:																		\
				if(strcmp(addr_csv, addr) == 0)								\
				{																	\
					printf("\nAddress found at Entry number %d\n", idx + 1);		\
					func_display_details(idx, adrsInfo);						\
					flag_found = 1;												\
				}																	\
																					\
				break;																\
																					\
			case e_phone:																		\
				if(strcmp(phone_csv, phone) == 0)								\
				{																	\
					printf("\nPhone found at Entry number %d\n", idx + 1);			\
					func_display_details(idx, adrsInfo);						\
					flag_found = 1;												\
				}																	\
																					\
				break;																\
																					\
			case e_age:																			\
				if(age_csv == age)											\
				{																	\
					printf("\nAge found at Entry number %d\n", idx + 1);			\
					func_display_details(idx, adrsInfo);						\
					flag_found = 1;												\
				}																	\
																					\
				break;																\
																					\
			case e_email:																		\
				if(strcmp(email_csv, email) == 0)								\
				{																	\
					printf("\nEmail found at Entry number %d\n", idx + 1);			\
					func_display_details(idx, adrsInfo);						\
					flag_found = 1;												\
				}																	\
																					\
				break;																\
																					\
			default:																			\
				printf("\nUnrecognized search type\n");								\
				break;																\
																					\
		} /* End of switch */														\
																					\
	} /* End of for loop */															\
																					\
	if(flag_found == 0)													\
	{																	\
		printf("\nNOT found !!!\n\n");								\
		return e_failure;												\
	}																	\
																					\
} /* End of macro */																



/* Macro to give user options to edit which all columns in a entry */
#define EDIT_OPTIONS													\
{																		\
			printf("Edit name?\n\tPress 1 for yes\n\t\t0 for no\n"); 	\
			scanf("%d", &yesno);										\
			if(yesno == 1)												\
			{															\
				printf("Enter the new name\n");							\
				__fpurge(stdin);										\
				scanf("%[a-zA-Z ]", new_name);							\
				strcpy(adrsInfo[idx].name, new_name);					\
			}															\
																		\
																		\
			printf("Edit address?\n\tPress 1 for yes\n\t\t0 for no\n"); \
			scanf("%d", &yesno);										\
			if(yesno == 1)												\
			{															\
				printf("Enter the new address\n");						\
				__fpurge(stdin);										\
				scanf("%[^\n]", new_addr);								\
				strcpy(adrsInfo[idx].addr, new_addr);					\
			}															\
																		\
			printf("Edit phone?\n\tPress 1 for yes\n\t\t0 for no\n");	\
			scanf("%d", &yesno);										\
			if(yesno == 1)												\
			{															\
				printf("Enter the new phone\n");						\
				__fpurge(stdin);										\
				scanf("%[^\n]", new_phone);								\
				strcpy(adrsInfo[idx].phone, new_phone);					\
			}															\
																		\
			printf("Edit age?\n\tPress 1 for yes\n\t\t0 for no\n");		\
			scanf("%d", &yesno);										\
			if(yesno == 1)												\
			{															\
				printf("Enter the new age\n");							\
				scanf("%hhu", &new_age);								\
				adrsInfo[idx].age = new_age;							\
			}															\
																		\
																		\
			printf("Edit email?\n\tPress 1 for yes\n\t\t0 for no\n");	\
			scanf("%d", &yesno);										\
			if(yesno == 1)												\
			{															\
				printf("Enter the new email\n");						\
				__fpurge(stdin);										\
				scanf("%[^\n]", new_email);								\
				strcpy(adrsInfo[idx].email, new_email);					\
			}															\
																		\
			__fpurge(stdin);											\
																		\
}


/* Macro to edit all other string type data viz. name, address, phone, email */
#define CASE_EDIT_BY(edit_by) 											\
{																		\
	int idx;															\
	int flag_found = 0;													\
	char new_name[SZ_NAME];												\
	char new_addr[SZ_NAME];												\
	char new_phone[SZ_PHONE];											\
	unsigned char new_age;												\
	char new_email[SZ_EMAIL];											\
																		\
	printf("Enter the search key as : " #edit_by "\n");					\
	scanf("%s", edit_by);												\
	for(idx = 0; idx < sno; idx++)										\
	{																	\
		if(strcmp(edit_by, adrsInfo[idx].edit_by) == 0)					\
		{																\
			flag_found = 1;												\
																		\
			printf(#edit_by " found at Entry number %d\n", idx + 1);	\
			func_display_details(idx, adrsInfo);						\
																		\
			EDIT_OPTIONS;												\
																		\
		}																\
	}																	\
																		\
																		\
	if(flag_found == 0)													\
	{																	\
		printf(#edit_by "NOT found\n");									\
		return e_failure;												\
	}																	\
}


/* Macro to delete entry for user given input key of age */
#define CASE_DELETE_BY_AGE												\
{																		\
	int idx;															\
	int flag_found = 0;													\
	printf("Enter age entry to be searched\n");							\
	scanf("%hhu", &age);												\
	for(idx = 0; idx < sno; idx++)										\
	{																	\
		if(age == adrsInfo[idx].age)									\
		{																\
			printf("age found at Entry number %d\n", idx + 1);			\
			func_delete_entry(idx, adrsInfo);							\
			flag_found = 1;												\
		}																\
	}																	\
	if(flag_found == 0)													\
	{																	\
		printf("age NOT found\n");										\
		return e_failure;												\
	}																	\
}


/* Macro to delete entry on user given input data of string type viz. name, address, phone, email */
#define CASE_DELETE_BY(delete_item) 									\
{																		\
	int idx;															\
	int flag_found = 0;													\
	printf("Enter " #delete_item " entry to be searched\n");			\
	scanf("%s", delete_item);											\
	for(idx = 0; idx < sno; idx++)										\
	{																	\
		if(strcmp(delete_item, adrsInfo[idx].delete_item) == 0)			\
		{																\
			printf(#delete_item " found at Entry number %d\n", idx + 1);\
			func_delete_entry(idx, adrsInfo);							\
			flag_found = 1;												\
		}																\
	}																	\
	if(flag_found == 0)													\
	{																	\
		printf(#delete_item "NOT found\n");								\
		return e_failure;												\
	}																	\
}

/* Userdefined funtions */

/* Display main menu and get operation type from user
 * Input : User Input for operation
 * Output : Main menu
 * Return value : Operation Type
 */
OperationType func_display_main_menu_and_get_operation_type(void)
{
	int operation;

	printf("Select Option \n");
	printf("  0. Add \n");
	printf("  1. Edit\n");
	printf("  2. Search \n");
	printf("  3. Delete \n");
	printf("  4. Display \n");
	printf("  5. Save \n");
	printf("  6. Exit \n");

	printf("\n");

	scanf("%d", &operation);
	if(operation >= 0 && operation <= 6)
		return operation;
	else
		return e_unrecognized;
}



/* Perform Add, Edit, Search, Delete, Display, Save or Exit as per user input
 * Input : Operation type, User input for various fields
 * Output : temporary address book structure array or address book file in .csv format
 * Return value : e_success or e_failure
 */
Status func_operation(OperationType op, AddrsBookInfo *adrsInfo)
{
	switch(op)
	{
		case e_add:
			func_add(adrsInfo);
			sno++;		// After every addition, the serial number is increased
			break;

		case e_edit:
			func_edit(adrsInfo);
			break;

		case e_search:
			func_search(adrsInfo);
			break;

		case e_delete:
			func_delete(adrsInfo);
			break;

		case e_display:
			if(func_display() == e_failure)
			{
				printf("Function %s failed\n", "func_display");
				return e_failure;
			}
			break;

		case e_save:
			if(func_save(adrsInfo) == e_failure) 
			{
				printf("Function %s failed\n", "func_save");
				return e_failure;
			}
			break;

		case e_exit:
			func_exit();
			break;

		default:
			printf("Unsupported operation");
			break;
			
	}
}


/* Add a new entry
 * Input : User input
 * Output : .csv file
 * Return value : e_success or e_failure
 */
Status func_add(AddrsBookInfo *adrsInfo)
{
	printf("Function add\n");
	printf("Entry No : %u\n", sno);

	__fpurge(stdin);
	printf("Enter Name\n");
	scanf("%[a-zA-Z ]", adrsInfo[sno].name);

	__fpurge(stdin);
	printf("Enter Address\n");
	scanf("%[^\n]", adrsInfo[sno].addr);

	__fpurge(stdin);
	printf("Enter Contact Number\n");
	scanf("%[^\n]", adrsInfo[sno].phone);

	__fpurge(stdin);
	printf("Enter Age\n");
	scanf("%hhu", &(adrsInfo[sno].age));

	__fpurge(stdin);
	printf("Enter email\n");
	scanf("%[^\n]", adrsInfo[sno].email);

	__fpurge(stdin);
	printf("\n");

	return e_success;
}


/* Edit a entry based on a search key and user input
 * Input : search key and user input
 * Output : temporary address book structure array
 * Return value : e_success or e_failure
 */
Status func_edit(AddrsBookInfo *adrsInfo)
{
	SearchType option;

	char name[SZ_NAME];
	char addr[SZ_ADDR];
	char phone[SZ_PHONE];
	unsigned char age;
	char email[SZ_EMAIL];

	int yesno;

	printf("Select Sub option for editing by \n");
	option = func_display_search_menu_and_get_option();

	printf("Sub option = %d\n", option);
	
	switch(option)
	{
		case e_name:
			;
			CASE_EDIT_BY(name);
			break;

		case e_address:
			;
			CASE_EDIT_BY(addr);
			break;

		case e_phone:
			;
			CASE_EDIT_BY(phone);
			break;

		case e_age:
			;
			CASE_EDIT_BY_AGE;
			break;

		case e_email:
			;
			CASE_EDIT_BY(email);
			break;

		default:
			printf("Wrong option\n");
			break;
	}

}


/* Search for a key in given field/column
 * Input : User input key
 * Output : found or not found
 * Return value : e_success or e_failure
 */
Status func_search(AddrsBookInfo *adrsInfo)
{
	SearchType option;

	printf("Select Sub option for searching\n");
	option = func_display_search_menu_and_get_option();

	printf("Sub option = %d\n", option);
	switch(option)
	{
		case e_name:
				;// This is just to insert a dummy statement, else gives error since there is a label in the previous line	
			CASE_SEARCH(e_name);
			break;

		case e_address:
				;	
			CASE_SEARCH(e_address);
			break;

		case e_phone:
				;	
			CASE_SEARCH(e_phone);
			break;

		case e_age:
				;
			CASE_SEARCH(e_age);
			break;

		case e_email:
				;	
			CASE_SEARCH(e_email);
			break;

		default:
			break;
	}

	return e_success;
}


/* Delete an entry
 * Input : User input key
 * Output : temporary address book structure array
 * Return value : e_success or e_failure
 */
Status func_delete(AddrsBookInfo *adrsInfo)
{
	SearchType option;
	char name[SZ_NAME];
	char addr[SZ_ADDR];
	char phone[SZ_PHONE];
	unsigned char age;
	char email[SZ_EMAIL];

	printf("Select Delete by\n");
	option = func_display_search_menu_and_get_option();

	printf("Sub option = %d\n", option);
	switch(option)
	{
		case e_name:
				;// This is just to insert a dummy statement, else gives error since there is a label in the previous line	
			CASE_DELETE_BY(name);
			break;

		case e_address:
			;
			CASE_DELETE_BY(addr);
			break;

		case e_phone:
				;	
			CASE_DELETE_BY(phone);
			break;

		case e_age:
				;
		 	CASE_DELETE_BY_AGE;	
			break;

		case e_email:
				; 	
			CASE_DELETE_BY(email);
			break;

		default:
			break;
	}

	return e_success;
}


/* Actual deletion of an entry, and remaining entries shifted back by one position
 * Input : Entry number (as index)
 * Output : Entry deleted
 * Return value : e_success or e_failure
 */
Status func_delete_entry(int idx, AddrsBookInfo *adrsInfo)						
{
	// If last entry to be deleted
	if(idx == sno - 1)
	{
		sno--;	// One entry less
	}
	// Else shift back each entry from idx position
	else
	{
		for( ;idx < sno - 1; idx++)
		{
			strcpy(adrsInfo[idx].name, adrsInfo[idx + 1].name);
			strcpy(adrsInfo[idx].addr, adrsInfo[idx + 1].addr);
			strcpy(adrsInfo[idx].phone, adrsInfo[idx + 1].phone);
			adrsInfo[idx].age = adrsInfo[idx + 1].age;
			strcpy(adrsInfo[idx].email, adrsInfo[idx + 1].email);
		}

		sno--;	// One entry less
	}
	
	printf("\nSuccessfully Deleted\n\n");

	return e_success;
}


/* Display details of an entry in human readable format
 * Input : Entry number (as index)
 * Output : Entry details
 * Return value : e_success or e_failure
 */
Status func_display_details(int idx, AddrsBookInfo *adrsInfo)
{
	printf("\t##%d##\n", idx + 1);
	printf("Name    : %s\n", adrsInfo[idx].name);
	printf("Address : %s\n", adrsInfo[idx].addr);
	printf("Phone   : %s\n", adrsInfo[idx].phone);
	printf("Age     : %d\n", adrsInfo[idx].age);
	printf("Email   : %s\n", adrsInfo[idx].email);

	printf("\n");

	return e_success;
}


/* Display sub menu for search by key, and get user input option
 * Input : User input
 * Output : selected option for searching
 * Return value : SearchType
 */
SearchType func_display_search_menu_and_get_option(void)
{
	int option;

//	printf("Select Option \n");
	printf("  0. Name \n");
	printf("  1. Address \n");
	printf("  2. Phone \n");
	printf("  3. Age \n");
	printf("  4. Email \n");

	scanf("%d", &option);

	if(option >= 0 && option <= 4)
		return option;
	else
		return e_unrecognized_op;

}


/* Just display the contents of .csv file as it is
 * Input : .csv address book
 * Output : Contents of .csv file
 * Return value : e_success or e_failure
 */
Status func_display(void)
{
	extern FILE *fptr_dst;
	char ch;

	if(NULL == fptr_dst)
	{
		fprintf(stderr, "********** Nothing to display *********\n\n");
		return e_success;
	}

	fptr_dst = fopen(CSV_FILENAME, "r");
	// Error handling
	if(NULL == fptr_dst)
	{
		printf("Error : fopen failed to open file %s\n", CSV_FILENAME);
		return e_failure;
	}

	printf("\n*** Contents of %s ***\n", CSV_FILENAME);
	fseek(fptr_dst, 0L, SEEK_SET);

	while((ch = fgetc(fptr_dst)) != EOF)
	{
		putchar(ch);
	}

	printf("\n");

	fclose(fptr_dst);

	return e_success;
}


/* Save the temporary address book structure array into .csv file
 * Input : address book structure array
 * Output : address book in .csv file
 * Return value : e_success or e_failure
 */
Status func_save(AddrsBookInfo *adrsInfo)
{
	extern FILE *fptr_dst;

	int idx;
	char temp_buff[30];
	char buff_entry[SIZE_BUFF];

	fptr_dst = fopen(CSV_FILENAME, "w+");
	// Error handling
	if(NULL == fptr_dst)
	{
		printf("Error : fopen failed to open file %s\n", CSV_FILENAME);
		return e_failure;
	}


	// fseek(fptr_dst, 0L, SEEK_SET);	// To bring the pointer to start of file every time fun_save is called

	for(idx = 0; idx < sno; idx++)
	{
		strcpy(buff_entry, "");

		sprintf(temp_buff, "%u,", idx + 1);
		strcat(buff_entry, temp_buff);
		sprintf(temp_buff, "%s,", adrsInfo[idx].name);
		strcat(buff_entry, temp_buff);
		sprintf(temp_buff, "%s,", adrsInfo[idx].addr);
		strcat(buff_entry, temp_buff);
		sprintf(temp_buff, "%s,", adrsInfo[idx].phone);
		strcat(buff_entry, temp_buff);
		sprintf(temp_buff, "%hhu,", adrsInfo[idx].age);
		strcat(buff_entry, temp_buff);
		sprintf(temp_buff, "%s,", adrsInfo[idx].email);
		strcat(buff_entry, temp_buff);

		fprintf(fptr_dst, "%s\n", buff_entry);

	}

	fclose(fptr_dst);

	printf("\nSuccessfully Saved\n\n");

	return e_success;
}


/* Cleanup and Exit program
 * Input : if any opened file pointers
 * Output : closed file pointers
 * Return value : void
 */
void func_exit(void)
{
	fcloseall();
	exit(1);
}
