#ifndef APC_H
#define APC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <math.h>

#define SUCCESS        0
#define FAILURE        1
#define LIST_EMPTY     2
#define DATA_NOT_FOUND 3

#define NODE_SIZE      2

typedef int data_t;

typedef struct _APC_t
{
	data_t data;
	struct _APC_t *prev, *next;
} APC_t;

int print_list      (APC_t *head);

int count_from_tail (APC_t *tail);

int insert_at_last  (APC_t **head, APC_t **tail, data_t data);

int add_prod        (APC_t **head, APC_t **tail, APC_t **curr, data_t data);

int delete_list     (APC_t **head, APC_t **tail);


int insert_at_first (APC_t **head, APC_t **tail, data_t data);

int delete_at_last  (APC_t **head, APC_t **tail);

int delete_at_first (APC_t **head, APC_t **tail);

int insert_after    (APC_t **head, APC_t **tail, data_t g_data, data_t a_data);

int insert_before   (APC_t **head, APC_t **tail, data_t g_data, data_t a_data);

APC_t *find_mid     (APC_t *head);

APC_t *find_nth_from_last (APC_t *head, int n);

#endif
