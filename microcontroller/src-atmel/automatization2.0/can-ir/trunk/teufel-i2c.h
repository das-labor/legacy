#ifndef _TEUFEL_I2C_H
#define _TEUFEL_I2C_H

#include <stdint.h>

typedef struct
{
	uint8_t vol;
	uint8_t id;
} t_channel;

extern void setAllChannels(uint8_t);
static void writeChannel(t_channel *);
extern void setDefaultAfterPoweron(void);
extern void TeufelSendCANPacket(void);
extern void setIncrementChannels(int8_t);

#endif // _TEUFEL_I2C_H
