#include <stdint.h>
#include "adc.h"

#pragma once

#define MOTOR_SAMPLE_READY     0x01

void motor_set_sampleval (uint16_t in_val);
void motor_tick (void);
void motor_init (void);
