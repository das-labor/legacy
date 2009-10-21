/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define DATA PD6
#define CLK  PD7
#define LAMPS 50
#define OUTPORT PORTD

#define BITSPERLAMP 12

typedef struct {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
} element_t;

void update()
{
	uint8_t i;
	_delay_us(10);
	for (i = 0; i < 5; i++)
		{
			PORTD |= _BV(DATA);
			PORTD &= ~_BV(DATA);
		}
}


void element_set(element_t *myel)
{
	uint8_t k;
	for (k = 0 ; k< BITSPERLAMP; k++)
		{
			OUTPORT = (((myel->blue & _BV(k)) >> k) << DATA);
			PORTD |= _BV(CLK);
			PORTD &= ~_BV(CLK);
		}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		OUTPORT = (((myel->green & _BV(k)) >> k) << DATA);
		PORTD |= _BV(CLK);
		PORTD &= ~_BV(CLK);
	}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		OUTPORT = (((myel->red & _BV(k)) >> k) << DATA);
		PORTD |= _BV(CLK);
		PORTD &= ~_BV(CLK);
	}
}

void band_shift(element_t *band, uint8_t size)
{
	element_t tmp_el = {0, 0, 0};
	uint8_t i;
	tmp_el.red = band[size-1].red;
	tmp_el.green = band[size-1].green;
	tmp_el.blue = band[size-1].blue;
	for (i = (size-1); i > 0; i--)
		{
			band[i].red = band[i-1].red;
			band[i].green = band[i-1].green;
			band[i].blue = band[i-1].blue;
		}
	band[0].red=tmp_el.red;
	band[0].green=tmp_el.green;
	band[0].blue=tmp_el.blue;  
}

void band_shiftback(element_t *band, uint8_t size)
{
	element_t tmp_el = {0, 0, 0};
	uint8_t i;
	tmp_el.red = band[0].red;
	tmp_el.green = band[0].green;
	tmp_el.blue = band[0].blue;

	for (i = 0; i < (size-1) ; i++)
		{
			band[i].red = band[i+1].red;
			band[i].green = band[i+1].green;
			band[i].blue = band[i+1].blue;
		}
	band[size-1].red=tmp_el.red;
	band[size-1].green=tmp_el.green;
	band[size-1].blue=tmp_el.blue;  
}

void band_redraw(element_t *band,uint8_t size)
{
	uint8_t i;
	for (i = 0; i < size; i++)
		{
			element_set(&(band[i]));
		}
	update();
}

/*
  ball ist schon auf dem feld
*/
void band_pingpong(element_t *band,uint8_t size,uint8_t ballsize,uint16_t speed)
{
	uint8_t i;
	band_redraw(band,size);
	_delay_ms(speed);
	for(i=ballsize-1;i<size;i++)
		{
			band_shift(band,size);
			band_redraw(band,size);
			_delay_ms(speed);
		}
	_delay_ms(1000);
	for(i=ballsize-1;i<size;i++)
		{ 
			band_shiftback(band,size);
			band_redraw(band,size);
			_delay_ms(speed);
		}
}

/*
  ball ist schon auf dem feld
*/
void band_pingpong_back(element_t *band,uint8_t size,uint8_t ballsize,uint16_t speed)
{
	uint8_t i;
	band_redraw(band,size);
	_delay_ms(speed);
	for(i=ballsize-1;i<size;i++)
		{ 
			band_shiftback(band,size);
			band_redraw(band,size);
			_delay_ms(speed);
		}
	_delay_ms(1000);
	for(i=ballsize-1;i<size;i++)
		{
			band_shift(band,size);
			band_redraw(band,size);
			_delay_ms(speed);
		}
}

void band_insert(element_t *final, uint8_t pos, element_t *toinsert, uint8_t size)
{
	uint8_t i;
	for(i=0;i<size;i++)
		{
			final[i+(pos)].red = toinsert[i].red;
			final[i+(pos)].green = toinsert[i].green;
			final[i+(pos)].blue = toinsert[i].blue;
		}
}

void band_epi(element_t *band, uint8_t size)
{
	uint8_t i;
		for(i=0;i<size;i++)
			{
				band[i].red=0x0FFF;
				band[i].green=0;
				band[i].blue=0x0000;
			}
		band_redraw(band,size);
		_delay_ms(20);
		for(i=0;i<size;i++)
			{
				band[i].red=0x0FFF;
				band[i].green=0x0FFF;
				band[i].blue=0x0000;
			}
		band_redraw(band,size);
		_delay_ms(20);
}

int main(void)
{
	element_t myband[25];
	element_t myband2[25];
	element_t myfinal[LAMPS];
	uint8_t dire=0;
	uint8_t counter=0;
	DDRD |= _BV(DATA) | _BV(CLK);
	PORTD |= _BV(DATA) | _BV(CLK);
//	int i, j, k, x = 0;
	int8_t i;
	for(i=0;i<LAMPS;i++)
		{
			myband[i].red=0;
			myband[i].green=0;
			myband[i].blue=0x0FFF;
		}
	myband[22].red=0x0FFF;
	myband[22].green=0x0000;
	myband[22].blue=0x0000;
	myband[23].red=0x0FFF;
	myband[23].green=0x0FFF;
	myband[23].blue=0x0FFF;
	myband[24].red=0x0FFF;
	myband[24].green=0x0000;
	myband[24].blue=0x0000;

	myband2[0].red=0x0000;
	myband2[0].green=0x0FFF;
	myband2[0].blue=0x0000;
	myband2[1].red=0x0FFF;
	myband2[1].green=0x0FFF;
	myband2[1].blue=0x0FFF;
	myband2[2].red=0x0000;
	myband2[2].green=0x0FFF;
	myband2[2].blue=0x0000;
	band_insert(myfinal,0,myband,25);
	band_insert(myfinal,25,myband2,25);
	band_redraw(myfinal,LAMPS);
	_delay_ms(1000);
	while (1)
	{
		if(counter == 22){
			counter=0;
			_delay_ms(1000);
			if (dire == 0) dire = 1;
			else dire=0;
		}
		counter++;
		if (dire == 0 )
			{
				band_shiftback(myband,25);
				band_shift(myband2,25);
			}
		else 
			{
				band_shift(myband,25);
				band_shiftback(myband2,25);
			}
		band_insert(myfinal,0,myband,25);
		band_insert(myfinal,25,myband2,25);
		
		band_redraw(myfinal,LAMPS);

		_delay_ms(50);
		//		band_pingpong(myband,LAMPS,4,10);
		//		_delay_ms(1000);
	}
	return 0;
}
