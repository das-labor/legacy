/************************
* this crc function is taken from the avr-libc documentation
* license is gpl v2
************************/

#include "crc.h"

uint16_t crc16_update(uint16_t crc, uint8_t a)
{
    int i;

    crc ^= a;
    for (i = 0; i < 8; ++i)
    {
        if (crc & 1)
            crc = (crc >> 1) ^ 0xA001;
        else
            crc = (crc >> 1);
    }

    return crc;
}

uint16_t calc_crc(uint8_t * buf, uint_fast8_t buflen)
{
    uint_fast8_t runner;
    uint16_t crc16 = 0;

    for(runner = 0; runner < buflen; runner++)
    {
        crc16 = crc16_update(crc16, buf[runner]);
    }

    return crc16;
}
