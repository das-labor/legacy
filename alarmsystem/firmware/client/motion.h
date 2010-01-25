#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>

void motiond_tick ();
uint16_t get_adc (uint8_t in_num);
uint8_t motion_check();
void motion_set_refval();
void mtd_disable();
void mtd_enable();

#define MTD_STATE_DISABLED 0x00
#define MTD_STATE_ENABLED 0x01
#define MTD_TRESHOLD 0x40
#define MTD_MINVAL 0x10

#endif /* MOTION_H */
