#include "femtoos_code.h"
#ifdef TWI_MTHREAD
/*******************************************************
 Author:					Manfred Langemann
                                                Alexander Kasper - ported to femtoos
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Master.h
 Description:    			TWI Master functions include file

 ********************************************************/
#ifndef _TWIM_H
#define _TWIM_H

#include <util/twi.h>

extern Tuint08 TWIM_Init(); // is in config_application
extern Tuint08 TWIM_Start(Tuint08 address, Tuint08 TWIM_Type);
extern void TWIM_Stop(void);
extern Tuint08 TWIM_Write(Tuint08 byte);
extern Tuint08 TWIM_ReadAck(void);
extern Tuint08 TWIM_ReadNack(void);


#define PROCESSI2CNONE  (0b00000000)
#define PROCESSI2CREAD  (0b00000001)
#define PROCESSI2CWRITE (0b00000010)

#define EI2CNONE        (0b00000000)
#define EI2CREAD        (0b00000001)
#define EI2CWRITE       (0b00000010)


typedef Tuint08 i2c_addr;

typedef struct {
  i2c_addr      addr_dst;
  Tuint08       process;
  Tuint08       dlc;
  Tuint08       data[8];

} i2c_message;

#endif

#endif
