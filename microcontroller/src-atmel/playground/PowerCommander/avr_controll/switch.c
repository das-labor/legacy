#include "PowerCommander.h"

#include <avr/io.h>
uint8_t switch00_off(struct t_status * data)
{
  PORTC &= ~_BV(PC3);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch00_on(struct t_status * data)
{
  PORTC |= _BV(PC3);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch00_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC3) & 1;
  (*data).write_data = 1;
  return 0;
}

uint8_t switch01_off(struct t_status * data)
{
  PORTC &= ~_BV(PC2);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch01_on(struct t_status * data)
{
  PORTC |= _BV(PC2);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch01_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC2) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch02_off(struct t_status * data)
{
  PORTC &= ~_BV(PC1);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch02_on(struct t_status * data)
{
  PORTC |= _BV(PC1);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch02_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC1) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch03_off(struct t_status * data)
{
  PORTC &= ~_BV(PC0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch03_on(struct t_status * data)
{
  PORTC |= _BV(PC0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch03_status(struct t_status * data)
{
  (*data).data=(PORTC >> PC0) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch04_off(struct t_status * data)
{
  PORTB &= ~_BV(PB5);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch04_on(struct t_status * data)
{
  PORTB |= _BV(PB5);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch04_status(struct t_status * data)
{
  (*data).data=(PORTB >> PB5) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch05_off(struct t_status * data)
{
  PORTB &= ~_BV(PB4);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch05_on(struct t_status * data)
{
  PORTB |= _BV(PB4);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch05_status(struct t_status * data)
{
  (*data).data=(PORTB >> PB4) & 1;
  (*data).write_data = 1;
  return 0;
}

uint8_t switch06_off(struct t_status * data)
{
  PORTB &= ~_BV(PB0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch06_on(struct t_status * data)
{
  PORTB |= _BV(PB0);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch06_status(struct t_status * data)
{
  (*data).data=(PORTB >> PB0) & 1;
  (*data).write_data = 1;
  return 0;
}


uint8_t switch07_off(struct t_status * data)
{
  PORTD &= ~_BV(PD7);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch07_on(struct t_status * data)
{
  PORTD |= _BV(PD7);
  (*data).write_data = 0;
  return 0;
}

uint8_t switch07_status(struct t_status * data)
{
  (*data).data=(PORTD >> PD7) & 1;
  (*data).write_data = 1;
  return 0;
}
