//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

#include <stdint.h>
#include <stdlib.h>
#include "list.h"

#define LIST_ALLOCATION_BLOCK_SIZE 8

void init_list(list_t * l){
	l->childs = malloc(sizeof(uint16_t) * LIST_ALLOCATION_BLOCK_SIZE );
	l->allocated = LIST_ALLOCATION_BLOCK_SIZE;
	l->num_childs = 0;
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

void list_foreach_begin(list_t * l){
	l->akt_foreach = 0;
}

void * list_foreach(list_t * l){
	void * e = 0;
	if(l->akt_foreach < l->num_childs){
		e = l->childs[l->akt_foreach];
		l->akt_foreach ++;
	}
	return e;
}

