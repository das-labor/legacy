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

#define COLOR_SET (0x00) 
#define COLOR_ADD ((0x01) << BITSPERLAMP)
#define COLOR_SUB ((0x02) << BITSPERLAMP)

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
			OUTPORT = ( ( (myel->blue >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
			PORTD |= _BV(CLK);
			PORTD &= ~_BV(CLK);
		}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		OUTPORT = ( ( (myel->green >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
		PORTD |= _BV(CLK);
		PORTD &= ~_BV(CLK);
	}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		OUTPORT = ( ( (myel->red >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
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

/*
	hier ueberlagern wir zwei felder. ein basisfeld aka hintergrund
	(final) und ein neues element (toinsert)

	es werden 12Bits zur farbdarstellung benutzt. d.h. wir haben 4 Bits
	fuer weitere Optionen aka wie diese beiden objekte zusammengefuehrt werden sollen
	
  00 01   : addition -> addiere toinsert zu final hinzu
  00 10   : subtraction
	default : copy -> von toinsert nach final kopieren
 */
void band_insert(element_t *final, uint8_t pos, element_t *toinsert, uint8_t size)
{
	uint8_t i;
	uint8_t k;
	for(i=0;i<size;i++)
		{
			k = ((toinsert[i].red) >> BITSPERLAMP);
			switch(k)
			{
			case 0x01:
				{
					final[i+(pos)].red = final[i+(pos)].red + (toinsert[i].red & 0x0FFF);
				}
				break;
			case 0x02:
				{
					final[i+(pos)].red = final[i+(pos)].red - (toinsert[i].red & 0x0FFF);
				}
				break;
			default:
				{
					final[i+(pos)].red = (toinsert[i].red & 0x0FFF);
				}
				break;
			}

			k = ((toinsert[i].green) >> BITSPERLAMP);
			switch(k)
			{
			case 0x01:
				{
					final[i+(pos)].green = final[i+(pos)].green + (toinsert[i].green & 0x0FFF);
				}
				break;
			case 0x02:
				{
					final[i+(pos)].green = final[i+(pos)].green - (toinsert[i].green & 0x0FFF);
				}
				break;
			default:
				{
					final[i+(pos)].green = (toinsert[i].green & 0x0FFF);
				}
				break;
			}

			k = ((toinsert[i].green) >> BITSPERLAMP);
			switch(k)
			{
			case 0x01:
				{
					final[i+(pos)].blue = final[i+(pos)].blue + (toinsert[i].blue & 0x0FFF);
				}
				break;
			case 0x02:
				{
					final[i+(pos)].blue = final[i+(pos)].blue - (toinsert[i].blue & 0x0FFF);
				}
				break;
			default:
				{
					final[i+(pos)].blue = (toinsert[i].blue & 0x0FFF);
				}
				break;
			}

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
/* 		for(i=0;i<size;i++) */
/* 			{ */
/* 				band[i].red=0x0FFF; */
/* 				band[i].green=0x0FFF; */
/* 				band[i].blue=0x0000; */
/* 			} */
/* 		band_redraw(band,size); */
/* 		_delay_ms(20); */
		for(i=0;i<size;i++)
			{
				band[i].red=0x0000;
				band[i].green=0;
				band[i].blue=0x0FFF;
			}
		band_redraw(band,size);
		_delay_ms(20);
		for(i=0;i<size;i++)
			{
				band[i].red=0x0000;
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
	int8_t dire=1;
	uint8_t counter=0;
	uint16_t bgvalue=0;
	DDRD |= _BV(DATA) | _BV(CLK);
	PORTD |= _BV(DATA) | _BV(CLK);
//	int i, j, k, x = 0;
	uint8_t i;
/* 	for(i=0;i<LAMPS;i++) */
/* 		{ */
/* 			myband[i].red=0; */
/* 			myband[i].green=0; */
/* 			myband[i].blue=0x0FFF; */
/* 		} */

	for ( i =0;i<25;i++)
		{
			myband[i].red = 0x0000 + (COLOR_ADD);
			myband[i].green = 0x0000 + (COLOR_ADD);
			myband[i].blue = 0x0000 + (COLOR_ADD);
			myband2[i].red = 0x0000 + (COLOR_ADD);
			myband2[i].green = 0x0000 + (COLOR_ADD);
			myband2[i].blue = 0x0000 + (COLOR_ADD);
		}

	myband[0].red=0x0AFF + (COLOR_SET);
	myband[0].green=0x0AFF + (COLOR_SET);
	myband[0].blue=0x0AFF + (COLOR_SET);
	myband[1].red=0x0AFF + (COLOR_SET);
	myband[1].green=0x0FFF + (COLOR_SET);
	myband[1].blue=0x0FFF + (COLOR_SET);
	myband[2].red=0x0AFF + (COLOR_SET);
	myband[2].green=0x0AFF + (COLOR_SET);
	myband[2].blue=0x0AFF + (COLOR_SET);

	myband2[22].red=0x0AFF + (COLOR_SET);
	myband2[22].green=0x0AFF + (COLOR_SET);
	myband2[22].blue=0x0AFF + (COLOR_SET);
	myband2[23].red=0x0FFF + (COLOR_SET);
	myband2[23].green=0x0FFF + (COLOR_SET);
	myband2[23].blue=0x0FFF + (COLOR_SET);
	myband2[24].red=0x0AFF + (COLOR_SET);
	myband2[24].green=0x0AFF + (COLOR_SET);
	myband2[24].blue=0x0AFF + (COLOR_SET);

	band_insert(myfinal,0,myband,25);
	band_insert(myfinal,25,myband2,25);
	band_redraw(myfinal,LAMPS);

	_delay_ms(1000);


	while (1)
	{
		bgvalue = bgvalue + (0x01F0*dire);
		if( bgvalue > 0x0f70 ) {
			dire= -1;
		}
		if (bgvalue < 0x00f0) {
			dire = 1;
		}

		//		band_epi(myfinal,LAMPS);

		if(counter == 25)
			{
				for ( i =0;i<25;i++)
					{
						myband[i].red = 0x0000 + (COLOR_ADD);
						myband[i].green = 0x0000 + (COLOR_ADD);
						myband[i].blue = 0x0000 + (COLOR_ADD);
						myband2[i].red = 0x0000 + (COLOR_ADD);
						myband2[i].green = 0x0000 + (COLOR_ADD);
						myband2[i].blue = 0x0000 + (COLOR_ADD);
					}
				myband[0].red=0x0AFF + (COLOR_SET);
				myband[0].green=0x0AFF + (COLOR_SET);
				myband[0].blue=0x0AFF + (COLOR_SET);
				myband[1].red=0x0AFF + (COLOR_SET);
				myband[1].green=0x0FFF + (COLOR_SET);
				myband[1].blue=0x0FFF + (COLOR_SET);
				myband[2].red=0x0AFF + (COLOR_SET);
				myband[2].green=0x0AFF + (COLOR_SET);
				myband[2].blue=0x0AFF + (COLOR_SET);

				myband2[22].red=0x0AFF + (COLOR_SET);
				myband2[22].green=0x0AFF + (COLOR_SET);
				myband2[22].blue=0x0AFF + (COLOR_SET);
				myband2[23].red=0x0FFF + (COLOR_SET);
				myband2[23].green=0x0FFF + (COLOR_SET);
				myband2[23].blue=0x0FFF + (COLOR_SET);
				myband2[24].red=0x0AFF + (COLOR_SET);
				myband2[24].green=0x0AFF + (COLOR_SET);
				myband2[24].blue=0x0AFF + (COLOR_SET);

				counter=0;
			//			_delay_ms(1000);
/* 			if (dire == 0) dire = 1; */
/* 			else dire=0; */
		}
		else 
			{
				band_shift(myband,25);
				band_shiftback(myband2,25);
			}
		counter++;

		for(i=0;i<LAMPS;i++)
			{
				myfinal[i].red=bgvalue;
				myfinal[i].green=0;
				myfinal[i].blue=0;
			}

		band_insert(myfinal,0,myband,25);
		band_insert(myfinal,25,myband2,25);

		band_redraw(myfinal,LAMPS);

		_delay_ms(30);

		//		band_pingpong(myband,LAMPS,4,10);
		//		_delay_ms(1000);
	}
	return 0;
}
