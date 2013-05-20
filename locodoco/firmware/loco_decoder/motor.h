#pragma once
#include <stdint.h>
#include "adc.h"
#include "config.h"
#include "../../common/protocol.h"
#include "rfm12.h"
#include "lock.h"

#define MOTOR_SAMPLE_READY     0x01

#define MOTOR_DIR_FW 0x00
#define MOTOR_DIR_RW 0x01

void motor_set_sampleval (uint16_t in_val);
void motor_tick (void);
void motor_init (void);
void motor_set_target_speed (uint16_t in_val);
void motor_set_speed_cmd (ldc_speed_t *in_s);
void motor_set_direction (uint8_t in_val);
