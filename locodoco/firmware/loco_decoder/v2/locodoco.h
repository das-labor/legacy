#include <avr/eeprom.h>
#include <avr/wdt.h>

#include "../../common/locodoco.h"
#include "config.h"
#include "motor.h"
#include "rfm12.h"

#pragma once


void locodoco_init (void);
void locodoco_tick ();
