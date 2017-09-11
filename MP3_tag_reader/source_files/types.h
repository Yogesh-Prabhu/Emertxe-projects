#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int u_int;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_read,
    e_edit,
    e_unsupported
} OperationType;

#endif
