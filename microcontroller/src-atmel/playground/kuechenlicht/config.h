#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#ifndef CONFIG_H
#define CONFIG_H

#define COLORS_INIT() DDRC |= (_BV(PC4) | _BV(PC5)); DDRD |= (_BV(PD0) | _BV(PD1))

#define RED_ON() (PORTC |= (_BV(PC4)))
#define RED_OFF() (PORTC &= ~(_BV(PC4)))

#define GREEN_ON() (PORTC |= (_BV(PC5)))
#define GREEN_OFF() (PORTC &= ~(_BV(PC5)))

#define BLUE_ON() (PORTD |= (_BV(PD1)))
#define BLUE_OFF() (PORTD &= ~(_BV(PD1)))

#define WHITE_ON() (PORTD |= (_BV(PD0)))
#define WHITE_OFF() (PORTD &= ~(_BV(PD0)))

#endif
