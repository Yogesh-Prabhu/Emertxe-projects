#ifndef TYPES_H
#define TYPES_H

#include "std_def.h"


/* User defined datatypes */
typedef unsigned int uint;

typedef enum
{
	e_success,
	e_failure
}Status;

typedef enum
{
	e_add,
	e_edit,
	e_search,
	e_delete,
	e_display,
	e_save,
	e_exit,
	e_unrecognized
}OperationType;

typedef enum
{
	e_name,
	e_address,
	e_phone,
	e_age,
	e_email,
	e_unrecognized_op
}SearchType;

typedef struct _AddrsBookInfo
{
	/*  */

	uint s_no;
	char name[SZ_NAME];
	char addr[SZ_ADDR];
	char phone[SZ_PHONE];
	unsigned char age;
	char email[SZ_EMAIL];

} AddrsBookInfo;




#endif
