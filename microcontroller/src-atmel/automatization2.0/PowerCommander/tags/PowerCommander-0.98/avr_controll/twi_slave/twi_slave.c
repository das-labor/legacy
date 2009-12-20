/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Slave.c
 Description:    			TWI slave functions

 -------------------------------------------------------
 Program Example
 -------------------------------------------------------
#include "General.h"
#include "TWI_Slave.h"

int main (void)
	{
	uint8_t		i=0;
	uint8_t		j=0;
	uint8_t		byte[8];
	uint8_t		TWIS_ResponseType;

 // Clear any interrupt
	cli ();

 // Wait 0.5 second for POR
	Delay_ms (500);

 // Initiate the TWI slave interface
	TWIS_Init (15, 100000);

 // Endless loop
	while (1)
		{

 // Is something to do for the TWI slave interface ?
		if (TWIS_ResponseRequired (&TWIS_ResponseType))
			{
			switch (TWIS_ResponseType)
				{
 // TWI requests to read a byte from the master.
 // It is implicitely assumed, that the master
 // sends 8 bytes.

				case TW_SR_SLA_ACK:
					for (i=0;i<7;i++)
						{
						byte[0] = TWIS_ReadAck ();
						}
					byte[7] = TWIS_ReadNack ();
					TWIS_Stop ();
					break;

 // TWI requests to write a byte to the master.
 // It is implicitely assumed, that the master
 // is prepared to receive 8 bytes.

				case TW_ST_SLA_ACK:
					for (i=0;i<8;i++)
						{                   
						TWIS_Write (j++);
						}
				    TWIS_Stop ();
					break;
				}
			}
 // Do something else ....... e.g.:
 		i++;
		}
	return 0;
	}
 ********************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "twi_slave.h"

/*******************************************************
 Public Function: TWIS_Init

 Purpose: Initialise the TWI Slave Interface

 Input Parameter:
  	- uint8_t	Slave address
 	- uint32_t	TWI_Bitrate (Hz)

 Return Value: uint8_t
 	- FALSE:	Bitrate too high
 	- TRUE:	Bitrate OK

*******************************************************/
uint8_t TWIS_Init(uint8_t Address, uint32_t Bitrate)
{
/*
** Set the TWI bitrate
** If TWBR is less 11, then error
*/
	TWBR = ((F_CPU/Bitrate) - 16) / 2;
	if (TWBR < 11)
		return 0;
/*
** Set the TWI slave address
*/
	TWAR = (Address << 1);
/*
** Activate TWI interface
*/
	TWCR = _BV(TWEN)|_BV(TWEA);

	return 1;
}
/*******************************************************
 Public Function: TWIS_Stop

 Purpose: Stop the TWI Slave Interface

 Input Parameter: None

 Return Value: None

*******************************************************/
void TWIS_Stop(void)
{
	TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO)|_BV(TWEA);
}
/*******************************************************
 Public Function: TWIS_Write

 Purpose: Write a byte to the master

 Input Parameter:
 	- uint8_t	Byte to be sent

*******************************************************/
void TWIS_Write(uint8_t byte)
{
	TWDR = byte;
	TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWEA);
	while (!(TWCR & _BV(TWINT)));
}
/*******************************************************
 Public Function: TWIS_ReadAck

 Purpose: Read a byte from the master and request next byte

 Input Parameter: None

 Return Value: uint8_t
  	- uint8_t	Read byte

*******************************************************/
uint8_t	TWIS_ReadAck(void)
{
	TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWEA);
	while (!(TWCR & _BV(TWINT)));
	return TWDR;
}
/*******************************************************
 Public Function: TWIS_ReadNack

 Purpose: Read the last byte from the master

 Input Parameter: None

 Return Value: uint8_t
  	- uint8_t	Read byte

*******************************************************/
uint8_t	TWIS_ReadNack(void)
{
	TWCR = _BV(TWINT)|_BV(TWEN);
	while (!(TWCR & _BV(TWINT)));
	return TWDR;
}
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
uint8_t TWIS_ResponseRequired(uint8_t *TWI_ResponseType)
{
	if (TWCR & _BV(TWINT))
	{
		*TWI_ResponseType = TW_STATUS;
		return 1;
	}

	return 0;
}
