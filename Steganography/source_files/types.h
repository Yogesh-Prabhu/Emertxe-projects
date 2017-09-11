#ifndef TYPES_H
#define TYPES_H

/* User defined datatypes */
typedef unsigned int	u_int;
typedef int				s_int;


typedef enum
{
	e_success,
	e_failure
} Status;

typedef enum
{
	e_encode,
	e_decode,
	e_unsupported
} OperationType;

#endif
