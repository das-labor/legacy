//buffer_handler.c

#include "config.h"

void resetbufferpointer(exposer_buffer_t *buf)
{
	buf->bufpointer = 0;
	buf->bitpointer = 0;
}

void append_n_one_bits(exposer_buffer_t *buf, uint16_t num)
{
	buf->bufpointer = &buf->buffer[0];
	buf->bufpointer += (buf->bitpointer>>3);
	
	while((buf->bitpointer < (EXPOSER_BUFFER_SIZE*8)) && (num > 0))
	{
		switch(buf->bitpointer&0x07)
		{
			case 0:
				*buf->bufpointer |= _BV(7);
				break;
			case 1:
				*buf->bufpointer |= _BV(6);
				break;
			case 2:
				*buf->bufpointer |= _BV(5);
				break;
			case 3:
				*buf->bufpointer |= _BV(4);
				break;
			case 4:
				*buf->bufpointer |= _BV(3);
				break;
			case 5:
				*buf->bufpointer |= _BV(2);
				break;
			case 6:
				*buf->bufpointer |= _BV(1);
				break;
			case 7:
				*buf->bufpointer |= _BV(0);
				buf->bufpointer ++;
				break;
		}
		buf->bitpointer++;
		num --;
	}
}

void append_n_zero_bits(exposer_buffer_t *buf, uint16_t num)
{
	buf->bufpointer = &buf->buffer[0];
	buf->bufpointer += (buf->bitpointer>>3);
	
	while((buf->bitpointer < (EXPOSER_BUFFER_SIZE*8)) && (num > 0))
	{
		switch(buf->bitpointer&0x07)
		{
			case 0:
				*buf->bufpointer &= ~_BV(7);
				break;
			case 1:
				*buf->bufpointer &= ~_BV(6);
				break;
			case 2:
				*buf->bufpointer &= ~_BV(5);
				break;
			case 3:
				*buf->bufpointer &= ~_BV(4);
				break;
			case 4:
				*buf->bufpointer &= ~_BV(3);
				break;
			case 5:
				*buf->bufpointer &= ~_BV(2);
				break;
			case 6:
				*buf->bufpointer &= ~_BV(1);
				break;
			case 7:
				*buf->bufpointer &= ~_BV(0);
				buf->bufpointer ++;
				break;
		}
		buf->bitpointer++;
		num --;
	}
}

void append_byte(exposer_buffer_t *buf,uint8_t data)
{
	buf->bufpointer = &buf->buffer[0];
	buf->bufpointer += (buf->bitpointer>>3);
	uint8_t num=8;
	uint8_t bit;
	while((buf->bitpointer < (EXPOSER_BUFFER_SIZE*8)) && (num > 0))
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