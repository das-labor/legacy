/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Slave.h
 Description:    			TWI slave functions include file

 ********************************************************/
#ifndef _TWIS
#define _TWIS

#include <util/twi.h>

/****************************************************************************
  Public Function definitions
****************************************************************************/
uint8_t	TWIS_Init(uint8_t address, uint32_t bitrate);
void	TWIS_Stop();
void	TWIS_Write(uint8_t byte);
uint8_t	TWIS_ReadAck();
uint8_t	TWIS_ReadNack();
uint8_t	TWIS_ResponseRequired(uint8_t *TWI_ResonseType);

#endif
