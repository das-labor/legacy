#include "motor.h"

#pragma once

/* adc daemon states */
#define ADCD_IDLE               0x00
#define ADCD_SAMPLE_MOTOR       0x01


void adcd_ctl (uint8_t in_state);
