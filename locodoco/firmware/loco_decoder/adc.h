#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"

/* adc daemon states */
#define ADCD_IDLE               0x00
#define ADCD_SAMPLE_MOTOR       0x01

void adcd_ctl (uint8_t in_state);
