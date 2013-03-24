/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Master.h
 Description:				TWI Master functions include file

 ********************************************************/
#ifndef _TWIM_H
#define _TWIM_H

#include <util/twi.h>
#include "../config.h"


// default bitrate
#ifndef TWI_BITRATE
#define TWI_BITRATE 100000UL
#endif

extern uint8_t TWIM_Init(void);
extern uint8_t TWIM_Start(uint8_t address);
extern void TWIM_Stop(void);
extern uint8_t TWIM_Write(uint8_t byte);
extern uint8_t TWIM_ReadAck(void);
extern uint8_t TWIM_ReadNack(void);

#endif // _TWIM_H

