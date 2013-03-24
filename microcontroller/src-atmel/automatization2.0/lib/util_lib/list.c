//
// This file is part of the gui-library for microcontrollers
// by Peter Fuhrmann.
//

#include <stdint.h>
#include <stdlib.h>

#include "list.h"

#define LIST_ALLOCATION_BLOCK_SIZE 4

void init_list(list_t *l)
{
	l->childs = malloc(sizeof(void *) * LIST_ALLOCATION_BLOCK_SIZE);
	l->allocated = LIST_ALLOCATION_BLOCK_SIZE;
	l->num_childs = 0;
}

//destructors
void uninit_list(list_t *l)
{
	free(l->childs);
}

void delete_list(list_t *l)
{
	free(l->childs);
	free(l);
}

list_t *new_list()
{
	list_t *l;
	l = malloc(sizeof(list_t));
	init_list(l);
	return l;
}

void list_append(list_t *l, void *child)
{
	if (l->num_childs >= l->allocated)
	{
		l->allocated += LIST_ALLOCATION_BLOCK_SIZE;
		l->childs = realloc(l->childs, sizeof(void *) * l->allocated);
	}

	l->childs[l->num_childs] = child;
	l->num_childs++;
}

//remove element from list by index
void list_remove_at(list_t *l, uint16_t i)
{
	if (i >= l->num_childs) return;//index to large

	l->num_childs -= 1;

	for (; i < l->num_childs; i++)
	{
		//move up all elements
		l->childs[i] = l->childs[i + 1];
	}
}

//remove element from list by value
void list_remove(list_t *l, void *child)
{
	uint16_t i;
	void *e = 0;

	for (i = 0; i < l->num_childs; i++)
	{
		e = l->childs[i];

		if (e == child)
		{
			//this is the child we want to remove
			list_remove_at(l, i);
		}
	}
}

void list_foreach_begin(list_iterator_t *it, list_t *l)
{
	l = l; //don't warn about unused list parameter
	*it = 0;
}

void *list_foreach(list_iterator_t *it, list_t *l)
{
	void *e = 0;

	if (*it < l->num_childs)
	{
		e = l->childs[*it];
		(*it)++;
	}

	return e;
}

