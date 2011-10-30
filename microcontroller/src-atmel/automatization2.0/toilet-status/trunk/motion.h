#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>

uint16_t filter (uint8_t in_idx, uint16_t in_val);
void motiond_tick ();
uint16_t get_adc (uint8_t in_num);
uint16_t motion_check();
void mtd_disable();
void mtd_enable();
void mtd_set_treshold (uint8_t in_t);
void mtd_init();
uint8_t mtd_get_treshold();

#define MTD_STATE_DISABLED 0x00
#define MTD_STATE_ENABLED 0x01

/* eeporm byte pos for treshold value */
#define MTD_EEPROM_BYTE (uint8_t *) 0x01

#endif /* MOTION_H */
