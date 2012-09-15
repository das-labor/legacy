#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#pragma once

#define MOTION_TRESHOLD 12
uint8_t motion_get (void);
void motion_init (void);
void motion_tick (void);

/* predefined ADC initialization values for motion detectors */
#define NUM_DETECTORS 2

/* pre-defined bits for ADMUX: 1.1V reference voltage */
#define ADMUX_PREDEF (_BV(REFS1) | _BV(REFS0))

