#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define DATA PD6
#define CLK  PD7
#define LAMPS 51
#define OUTPORT PORTD

#define BITSPERLAMP 12

typedef struct {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
} element_t;

element_t band[LAMPS];

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
	tmp_el.red = band[0].red;
	tmp_el.green = band[0].green;
	tmp_el.blue = band[0].blue;
	for (i = 0; i < size - 1; i++)
	{
		band[i].red = band[i+1].red;
		band[i].green = band[i+1].green;
		band[i].blue = band[i+1].blue;
	}
	band[size-1].red=tmp_el.red;
	band[size-1].green=tmp_el.green;
	band[size-1].blue=tmp_el.blue;  
}

void band_shiftback(element_t *band, uint8_t size)
{
	element_t tmp_el = {0, 0, 0};
	uint8_t i;
	tmp_el.red = band[0].red;
	tmp_el.green = band[0].green;
	tmp_el.blue = band[0].blue;

	for (i = 1; i < size ; i++)
	{
		band[i-1].red = band[i].red;
		band[i-1].green = band[i].green;
		band[i-1].blue = band[i].blue;
	}
	band[size-1].red=tmp_el.red;
	band[size-1].green=tmp_el.green;
	band[size-1].blue=tmp_el.blue;  
}

void band_redraw()
{
  uint8_t i;
  for (i = 0; i < LAMPS; i++)
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
  for(i=ballsize-1;i<size;i++){
    band_shift(band,size);
    band_redraw();
    _delay_ms(speed);
  }
  for(i=ballsize-1;i<size;i++){
    band_shiftback(band,size);
    band_redraw();
    _delay_ms(speed);
  }
  
}


int main(void)
{
	DDRD |= _BV(DATA) | _BV(CLK);
	PORTD |= _BV(DATA) | _BV(CLK);
//	int i, j, k, x = 0;
	uint8_t i;
	for(i=0;i<LAMPS;i++)
	  {
	    band[i].red=0;
	    band[i].green=0;
	    band[i].blue=0;
	  }
	band[0].red=0x0FFF;
	band[0].green=0x0FFF;
	band[0].blue=0x0FFF;
	while (1)
	{
	  band_redraw();
	  band_pingpong(band,LAMPS,1,100);
	  //	  band_shift(band,LAMPS);
	  //	  _delay_ms(250);

/*		for (k = 0; k < LAMPS; k++)
		{
			for (i = 0; i < 3; i++)
			{
				x++;
				for (j = 0; j < 12; j++)
				{
					if (((i == 0) && (x & 1)) || ((i == 1) && (x & 2)) || ((i == 2) && (x & 4)))
						PORTD &= ~_BV(DATA);
					else
						PORTD |= _BV(DATA);
					PORTD |= _BV(CLK);
					PORTD &= ~_BV(CLK);
				}
				x *= 3;
			}
		}
	
		// Trigger uebernehmen
		_delay_us(10);
		for (j = 0; j < 5; j++)
		{
			PORTD |= _BV(DATA);
			PORTD &= ~_BV(DATA);
		}
		_delay_ms(300);
	
		for (k = 0; k < LAMPS; k++)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 12; j++)
				{
					PORTD &= ~_BV(DATA);
					PORTD |= _BV(CLK);
//					_delay_us(2);
					PORTD &= ~_BV(CLK);
				}
			}
		}
		_delay_us(10);
		for (j = 0; j < 5; j++)
		{
			PORTD |= _BV(DATA);
//			_delay_us(2);
			PORTD &= ~_BV(DATA);
		}
		_delay_ms(300);*/
	}
	return 0;
}
