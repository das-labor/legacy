//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

#include <stdint.h>
#include <stdlib.h>
#include "list.h"

#define LIST_ALLOCATION_BLOCK_SIZE 4

void init_list(list_t * l){
	l->childs = malloc(sizeof(uint16_t) * LIST_ALLOCATION_BLOCK_SIZE );
	l->allocated = LIST_ALLOCATION_BLOCK_SIZE;
	l->num_childs = 0;
}

//destructors
void uninit_list(list_t * l){
	free(l->childs);
}

void delete_list (list_t * l) {
	free(l->childs);
	free(l);
}

list_t * new_list() {
	list_t * l;
	l = malloc(sizeof(list_t));
	return l;
}


void list_append(list_t * l, void * child) {
	if(l->num_childs >= l->allocated){
		l->allocated += LIST_ALLOCATION_BLOCK_SIZE;
		l->childs = realloc(l->childs, l->allocated);		
	}
	l->childs[l->num_childs] = child;
	l->num_childs++;
}

void list_foreach_begin(list_iterator_t * it, list_t * l){
	*it = 0;
}

void * list_foreach(list_iterator_t * it, list_t * l){
	void * e = 0;
	if(*it < l->num_childs){
		e = l->childs[*it];
		(*it)++;
	}
	return e;
}

