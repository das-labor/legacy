#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>

uint16_t crc16_update(uint16_t crc, uint8_t a);
uint16_t calc_crc(uint8_t * buf, uint_fast8_t buflen);

#endif
