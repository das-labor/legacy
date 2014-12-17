#ifndef _TEUFEL_I2C_H
#define _TEUFEL_I2C_H

#include <stdint.h>

extern void setDefaultAfterPoweron(void);


typedef struct
{
	uint8_t vol;
	uint8_t id;
} t_channel;


#endif // _TEUFEL_I2C_H
