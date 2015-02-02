#ifndef _TEUFEL_I2C_H
#define _TEUFEL_I2C_H

#include <stdint.h>

extern void setIncrementChannels(int8_t diff);
extern void setAllChannels(uint8_t vol);
extern void setSingleChannel(uint8_t chanID, uint8_t vol);

extern void TeufelSendCANPacket(void);

extern void setDefaultAfterPoweron(void);
extern void TeufelPoweron(void);
extern void TeufelPoweronTick(void);

#endif // _TEUFEL_I2C_H
