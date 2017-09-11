#ifndef ADD_H
#define ADD_H

#include "types.h"

/* User defined Function Prototypes */

/* Display main menu and get user selected opration*/
OperationType func_display_main_menu_and_get_operation_type(void);

/* Perform operations Add, Edit, Search, Delete, Display, Save or Exit as per user input */
Status func_operation(OperationType op, AddrsBookInfo *adrsInfo);

/* Add a new Entry */
Status func_add(AddrsBookInfo *adrsInfo);

/* Edit a entry based on a search key and user input */
Status func_edit(AddrsBookInfo *adrsInfo);

/* Search for a given key in a field/column */
Status func_search(AddrsBookInfo *adrsInfo);

/* Delete an entry */
Status func_delete(AddrsBookInfo *adrsInfo);

/* Actual deleting an entry and shifting the remaining entries back by one place */
Status func_delete_entry(int idx, AddrsBookInfo *adrsInfo);

/* Display details of an entry in human readable format */
Status func_display_details(int idx, AddrsBookInfo *adrsInfo);

/* Display sub menu for search by key, and get user input option */
SearchType func_display_search_menu_and_get_option(void);

/* Just display the contents of .csv file as it is */
Status func_display(void);

/* Save the temporary address book structure array into .csv file */
Status func_save(AddrsBookInfo *adrsInfo);

/* Cleanup and Exit */
void func_exit(void);






#endif
