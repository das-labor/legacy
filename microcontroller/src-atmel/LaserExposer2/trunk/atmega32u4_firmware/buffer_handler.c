//buffer_handler.c

#include <inttypes.h>
#include "config.h"

void resetbufferpointer(exposer_buffer_t *buf)
{
	buf->bufpointer = 0;
	buf->bitpointer = 0;
}

void append_n_one_bits(exposer_buffer_t *buf, uint16_t num)
{
	uint8_t register temp;
	buf->bufpointer = &buf->buffer[0];
	buf->bufpointer += (buf->bitpointer>>3);
	temp= *buf->bufpointer;

	if(((buf->bitpointer>>3) + num) > EXPOSER_BUFFER_SIZE)
		return;
	
	while(num > 0)
	{
		switch(buf->bitpointer&0x07)
		{
			case 0:
				while(num > 7)
				{
					*buf->bufpointer=0xff;
					buf->bufpointer ++;
					buf->bitpointer+=8;
					num-=8;
				}
				temp |= _BV(7);
				break;
			case 1:
				temp |= _BV(6);
				break;
			case 2:
				temp |= _BV(5);
				break;
			case 3:
				temp |= _BV(4);
				break;
			case 4:
				temp |= _BV(3);
				break;
			case 5:
				temp |= _BV(2);
				break;
			case 6:
				temp |= _BV(1);
				break;
			case 7:
				temp |= _BV(0);
				*buf->bufpointer=temp;
				buf->bufpointer ++;
				temp=0;
				break;
		}
		buf->bitpointer++;
		num --;
	}
	*buf->bufpointer=temp;
}

void append_n_zero_bits(exposer_buffer_t *buf, uint16_t num)
{
	uint8_t register temp;
	buf->bufpointer = &buf->buffer[0];
	buf->bufpointer += (buf->bitpointer>>3);
	temp= *buf->bufpointer;

	if(((buf->bitpointer>>3) + num) > EXPOSER_BUFFER_SIZE)
		return;
	
	while(num > 0)
	{
		switch(buf->bitpointer&0x07)
		{
			case 0:
				while(num > 7)
				{
					*buf->bufpointer=0x00;
					buf->bufpointer ++;
					buf->bitpointer+=8;
					num-=8;
				}
				temp &= ~_BV(7);
				break;
			case 1:
				temp &= ~_BV(6);
				break;
			case 2:
				temp &= ~_BV(5);
				break;
			case 3:
				temp &= ~_BV(4);
				break;
			case 4:
				temp &= ~_BV(3);
				break;
			case 5:
				temp &= ~_BV(2);
				break;
			case 6:
				temp &= ~_BV(1);
				break;
			case 7:
				temp &= ~_BV(0);
				*buf->bufpointer=temp;
				buf->bufpointer ++;
				temp=0;
				break;
		}
		buf->bitpointer++;
		num --;
	}
	*buf->bufpointer=temp;
}

void append_byte(exposer_buffer_t *buf,uint8_t data)
{
	
	buf->bufpointer = &buf->buffer[0];
	buf->bufpointer += (buf->bitpointer>>3);
	uint8_t num=8;
	uint8_t bit;

	if(((buf->bitpointer>>3) + num) > EXPOSER_BUFFER_SIZE)
		return;

	while(num > 0)
	{
		bit = data&(1<<(num-1));
		switch(buf->bitpointer&0x07)
		{
			case 0:
				if(bit)
					*buf->bufpointer |= _BV(7);
				else
					*buf->bufpointer &= ~_BV(7);
				break;
			case 1:
				if(bit)
					*buf->bufpointer |= _BV(6);
				else
					*buf->bufpointer &= ~_BV(6);
				break;
			case 2:
				if(bit)
					*buf->bufpointer |= _BV(5);
				else
					*buf->bufpointer &= ~_BV(5);
				break;
			case 3:
				if(bit)
					*buf->bufpointer |= _BV(4);
				else
					*buf->bufpointer &= ~_BV(4);
				break;
			case 4:
				if(bit)
					*buf->bufpointer |= _BV(3);
				else
					*buf->bufpointer &= ~_BV(3);
				break;
			case 5:
				if(bit)
					*buf->bufpointer |= _BV(2);
				else
					*buf->bufpointer &= ~_BV(2);
				break;
			case 6:
				if(bit)
					*buf->bufpointer |= _BV(1);
				else
					*buf->bufpointer &= ~_BV(1);
				break;
			case 7:
				if(bit)
					*buf->bufpointer |= _BV(0);
				else
					*buf->bufpointer &= ~_BV(0);
				buf->bufpointer ++;
				break;
		}
		buf->bitpointer++;
		num --;
	}

}

void set_byte(exposer_buffer_t *buf,uint16_t index, uint8_t data)
{
	if(index < EXPOSER_BUFFER_SIZE)
		buf->buffer[index]=data;
}
