/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>
#include "PowerCommander.h"



uint8_t bright_tafel_set(struct t_status *data)
{
	OCR0A = (*data).data;
	(*data).write_data=0;
	return 0;
}

uint8_t bright_beamer_set(struct t_status *data)
{
	OCR0B = (*data).data;
	(*data).write_data=0;
	return 0;
}

uint8_t bright_schraenke_set(struct t_status *data)
{
	OCR1A = (*data).data;
	(*data).write_data=0;
	return 0;
}

uint8_t bright_flipper_set(struct t_status *data)
{
	OCR1B = (*data).data;
	(*data).write_data=0;
	return 0;
}

uint8_t bright_free_set(struct t_status *data)
{
	OCR2A = (*data).data;
	(*data).write_data=0;
	return 0;
}


uint8_t bright_lounge_set(struct t_status *data)
{
	OCR2B = (*data).data;
	(*data).write_data=0;
	return 0;
}

uint8_t bright_vortrag_set(struct t_state_vortrag *data){
	struct t_status lampe = { (*data).bright_tafel, 0 };
	bright_tafel_set(&lampe);
	lampe.data = (*data).bright_beamer;
	bright_beamer_set(&lampe);
	lampe.data = (*data).bright_schraenke;
	bright_schraenke_set(&lampe);
	lampe.data = (*data).bright_flipper;
	bright_flipper_set(&lampe);
}
