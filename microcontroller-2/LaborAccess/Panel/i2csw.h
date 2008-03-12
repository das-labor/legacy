#ifndef I2CSW_H
#define I2CSW_H

#include "myint.h"

// include project-dependent settings
#include "i2cswconf.h"

// functions

void i2cStart();

void i2cStop();


u08 i2cPutbyte(u08 b);

u08 i2cGetbyte(u08 last);


void i2cInit(void);

//write to i2c eeprom in multibyte write mode
s08 i2cEeWrite(u16 address, u16 len, u08 *data);

//read bytes from eeprom to buffer
void i2cEeRead(u16 address, u16 len, u08 *data);

//detect if there is a valid i2c eeprom card inserted
//returns card size in sectors of 256 bytes
//a return value of zero means no valid card
u08 i2cEeDetect();


#endif
