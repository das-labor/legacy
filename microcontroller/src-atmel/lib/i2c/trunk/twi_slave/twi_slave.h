/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Slave.h
 Description:				TWI slave functions include file

 ********************************************************/
#ifndef _TWIS
#define _TWIS

#include <util/twi.h>
#include "../config.h"


// default address
#ifndef TWI_ADDRESS
#define TWI_ADDRESS 42
#endif

// read address from eeprom define in config.h
// #define TWI_EEPROM_ADDR 0

/****************************************************************************
  Public Function definitions
****************************************************************************/

/*******************************************************
 Public Function: TWIS_Init

 Purpose: Initialise the TWI Slave Interface

 Return Value: uint8_t
 	- FALSE:	Bitrate too high
 	- TRUE:	Bitrate OK

*******************************************************/
uint8_t TWIS_Init(void);

/*******************************************************
 Public Function: TWIS_Stop

 Purpose: Stop the TWI Slave Interface

*******************************************************/
void TWIS_Stop(void);

/*******************************************************
 Public Function: TWIS_Write

 Purpose: Write a byte to the master

 Input Parameter:
 	- uint8_t	Byte to be sent

*******************************************************/
void TWIS_Write(uint8_t byte);

/*******************************************************
 Public Function: TWIS_ReadAck

 Purpose: Read a byte from the master and request next byte

 Input Parameter: None

 Return Value: uint8_t
  	- uint8_t	Read byte

*******************************************************/
uint8_t TWIS_ReadAck(void);

/*******************************************************
 Public Function: TWIS_ReadNack

 Purpose: Read the last byte from the master

 Input Parameter: None

 Return Value: uint8_t
  	- uint8_t	Read byte

*******************************************************/
uint8_t TWIS_ReadNack(void);

/*******************************************************
 Public Function: TWIS_ResponseRequired

 Purpose: Get the response type to be performed by slave

 Input Parameter:
  	- uint8_t*	Pointer to response type
	on return:
		TWIS_ReadBytes	--> Read byte(s) from master
		TWIS_WriteBytes	--> Write byte(s) to master

 Return Value: uint8_t
  	Response required
		TRUE: Yes, response required
		FALSE: No response required

*******************************************************/
uint8_t TWIS_ResponseRequired(uint8_t *TWI_ResonseType);


#endif
