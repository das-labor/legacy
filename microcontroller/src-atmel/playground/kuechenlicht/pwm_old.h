#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#ifndef PWM_H
#define PWM_H

#include "config.h"
void fadeto (uint8_t r, uint8_t g, uint8_t b);
void fade_tick();
void pwm_init();
#endif
