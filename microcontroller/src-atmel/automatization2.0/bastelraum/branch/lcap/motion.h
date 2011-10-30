#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "config.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#pragma once

#define MOTION_TRESHOLD 64
uint8_t motion_get (void);
void motion_init (void);
void motion_tick (void);
