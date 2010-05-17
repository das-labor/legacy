#ifndef _I2C_FUNKTIONEN_H
#define _I2C_FUNKTIONEN_H

extern void sync_stat_cache();

extern void twi_send(uint8_t *p);

extern void twi_get(uint8_t *p);

#endif // _I2C_FUNKTIONEN
