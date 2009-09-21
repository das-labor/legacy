/* -*- Mode: C; tab-width: 2 -*- */
//#include <avr/io.h>
//#include "PowerCommander.h"


inline void bright_fkt(uint8_t port, struct t_busdata *data)	{
	switch( (*data).in_data[1])
		{
		case A_BR_SET:
			{
				port = (*data).in_data[2];
				(*data).write_data = HASNDATA;
			}
			break;
		case A_BR_GET:
			{
				(*data).out_data = port;
				(*data).write_data = HASDATA;
			}
			break;
		default:
			break;
		}
}

/* void bright_vortrag_set(struct t_state_vortrag *data){ */
/* 	struct t_status lampe = { (*data).bright_tafel, 0 }; */
/* 	bright_tafel_set(&lampe); */
/* 	lampe.data = (*data).bright_beamer; */
/* 	bright_beamer_set(&lampe); */
/* 	lampe.data = (*data).bright_schraenke; */
/* 	bright_schraenke_set(&lampe); */
/* 	lampe.data = (*data).bright_flipper; */
/* 	bright_flipper_set(&lampe); */
	
/* } */

/* void bright_lounge_state_set(struct t_state_lounge *data){ */
/* 	struct t_status lampe = { (*data).bright_lounge, 0 }; */
/* 	bright_lounge_set(&lampe); */
/* 	lampe.data = (*data).bright_free; */
/* 	bright_free_set(&lampe); */
/* } */
