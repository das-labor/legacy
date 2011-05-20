//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

#ifndef _H_LIST
#define _H_LIST

typedef struct {
	void ** childs;
	uint16_t num_childs;
	uint16_t allocated;
	uint16_t akt_foreach;
} list_t;


void init_list(list_t * l);

void delete_list(list_t * l);//destructor
void uninit_list(list_t * l);//destructor

list_t * new_list();
void list_append(list_t * l, void * child) ;


typedef uint16_t list_iterator_t;

void list_foreach_begin(list_iterator_t * it, list_t * l);
void * list_foreach(list_iterator_t * it, list_t * l);

#endif
