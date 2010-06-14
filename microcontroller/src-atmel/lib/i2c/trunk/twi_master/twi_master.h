/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Master.h
 Description:    			TWI Master functions include file

 ********************************************************/
#ifndef _TWIM_H
#define _TWIM_H

#include <util/twi.h>
#include "../config.h"


// default bitrate
#ifndef TWI_BITRATE
#define TWI_BITRATE 100000UL
#endif


uint8_t TWIM_Init();
uint8_t TWIM_Start(uint8_t address);
void TWIM_Stop();
uint8_t TWIM_Write(uint8_t byte);
uint8_t TWIM_ReadAck();
uint8_t TWIM_ReadNack();

#endif
