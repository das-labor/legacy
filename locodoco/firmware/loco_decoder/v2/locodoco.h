#include <avr/eeprom.h>
#include <avr/wdt.h>
#pragma once

#include "config.h"
#include "motor.h"
#include "rfm12.h"



void locodoco_init (void);
void locodoco_tick ();
