

#include "tlv5604.h"
#include "pl_dim.h"

#include <avr/pgmspace.h>

#include "exp_table.c"


//cal = (zeroval-maxval-800)*512/800
/*
typedef struct{
	uint16_t zeroval;
	uint16_t minval;
	uint16_t maxval;
	uint16_t preheat;
	int8_t cal;
}pl_param_t;
*/



pl_param_t pl[]={
	{0xff*4, 0xe2*4, 0x40*4, 0xc0*4, 38},
	{0xff*4, 0xf0*4, 0x50*4, 0xe0*4, 38},
	{0xff*4, 0xf1*4, 0x48*4, 0xe0*4, 38},
};


//returns the value to output on the d/a converter for a specific brightnes
uint16_t bright_calc(pl_param_t * param, uint8_t bright){
	uint16_t val;	
	val = pgm_read_word(&exp_table[bright]);
	val += (param->cal * (uint8_t)(val/4)) / 128;
	val = param->zeroval-val;
	if (val > param->minval) val = param->minval;
	if (val < param->maxval) val = param->maxval;
	return val;
}



uint8_t lampbright[8];

void lampe_dim(uint8_t lampe, int8_t d){
	int16_t tmp;
	tmp = lampbright[lampe] +d;
	if(tmp>0xff) tmp = 0xff;
	if(tmp<0) tmp = 0;
	lampbright[lampe] = tmp;
	
	da_set (lampe, bright_calc (&pl[lampe], tmp));
}

void lampe_set(uint8_t lampe, uint8_t bright){
	lampbright[lampe] = bright;
	da_set (lampe, bright_calc (&pl[lampe], bright));
}

void lampe_init(){
	//Anschaltsignale:
	//Unit 0 PB1 active low
	//andere : PB0 (active high)
	
	DDRB  |= 0x03;
	PORTB |= 0x01;
}
