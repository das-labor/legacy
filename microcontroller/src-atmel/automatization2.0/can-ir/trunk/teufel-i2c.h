#ifndef _TEUFEL_I2C_H
#define _TEUFEL_I2C_H

#include <stdint.h>

typedef struct
{
	uint8_t rr;
	uint8_t sub;
	uint8_t rl;
	uint8_t cen;
	uint8_t fr;
	uint8_t fl;
	uint8_t sl;
	uint8_t sr;
} t_vol;

extern void setDefaultAfterPoweron(void);

#endif // _TEUFEL_I2C_H
