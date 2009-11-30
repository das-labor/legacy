/* -*- Mode: C; tab-width: 2 -*- */
//#include "PowerCommander.h"

//#include <avr/io.h>



void switch_on(struct t_status * data){
	switch ((*data).data){
	case switch00:
		PORTC |= _BV(PC3);
		break;
	case switch01:
		PORTC |= _BV(PC2);
		break;
	case switch02:
		PORTC |= _BV(PC1);
		break;
	case switch03:
		PORTC |= _BV(PC0);
		break;
	case switch04:
		PORTB |= _BV(PB5);
		break;
	case switch05:
			PORTB |= _BV(PB4);
		break;
	case switch06:
		PORTB |= _BV(PB0);
		break;
	case switch07:
		PORTD |= _BV(PD7);
		break;
	case optokopp00:
		PORTD |= _BV(PD0);
		break;
	case optokopp01:
		PORTD |= _BV(PD1);
		break;
	default:
		break;
	}
	(*data).write_data = 0;
}

void switch_off(struct t_status * data){
	switch ((*data).data){
	case switch00:
		PORTC &= ~_BV(PC3);
		break;
	case switch01:
		PORTC &= ~_BV(PC2);
		break;
	case switch02:
		PORTC &= ~_BV(PC1);
		break;
	case switch03:
		PORTC &= ~_BV(PC0);
		break;
	case switch04:
		PORTB &= ~_BV(PB5);
		break;
	case switch05:
		PORTB &= ~_BV(PB4);
		break;
	case switch06:
		PORTB &= ~_BV(PB0);
		break;
	case switch07:
		PORTD &= ~_BV(PD7);
		break;
	case optokopp00:
		PORTD &= ~_BV(PD0);
		break;
	case optokopp01:
		PORTD &= ~_BV(PD1);
		break;
	default:
		break;
	}
	(*data).write_data = 0;
}

void switch_status(struct t_status * data){
	switch ((*data).data){
	case switch00:
		(*data).data=(PORTC >> PC3) & 1;
		break;
	case switch01:
		(*data).data=(PORTC >> PC2) & 1;
		break;
	case switch02:
		(*data).data=(PORTC >> PC1) & 1;
		break;
	case switch03:
		(*data).data=(PORTC >> PC0) & 1;
		break;
	case switch04:
		(*data).data=(PORTB >> PB5) & 1;
		break;
	case switch05:
		(*data).data=(PORTB >> PB4) & 1;
		break;
	case switch06:
		(*data).data=(PORTB >> PB0) & 1;
		break;
	case switch07:
		(*data).data=(PORTD >> PD7) & 1;
		break;
	case optokopp00:
		(*data).data=(PORTD >> PD0) & 1;
		break;
	case optokopp01:
		(*data).data=(PORTD >> PD1) & 1;
		break;
	default:
		break;
	}
	(*data).write_data = 1;
}
		

