#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>

uint16_t filter (uint8_t in_idx, uint16_t in_val);
void motiond_tick ();
uint16_t get_adc (uint8_t in_num);
uint8_t motion_check();
void mtd_disable();
void mtd_enable();

#define MTD_STATE_DISABLED 0x00
#define MTD_STATE_ENABLED 0x01
#define MTD_TRESHOLD 7
#define MTD_MINVAL 0x20

#endif /* MOTION_H */
