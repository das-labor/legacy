/* -*- Mode: C; tab-width: 2 -*- */
#include "PowerCommander.h"
#include <avr/io.h>
uint8_t opto00_off(struct t_status * data)
{
  PORTD &= ~_BV(PD0);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto00_on(struct t_status * data)
{
  PORTD |= _BV(PD0);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto00_status(struct t_status * data)
{
  (*data).data=(PORTD >> PD0) & 1;
  (*data).write_data = 1;
  return 0;
}

uint8_t opto01_off(struct t_status * data)
{
  PORTD &= ~_BV(PD1);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto01_on(struct t_status * data)
{
  PORTD |= _BV(PD1);
  (*data).write_data = 0;
  return 0;
}

uint8_t opto01_status(struct t_status * data)
{
  (*data).data=(PORTD >> PD1) & 1;
  (*data).write_data = 1;
  return 0;
}
