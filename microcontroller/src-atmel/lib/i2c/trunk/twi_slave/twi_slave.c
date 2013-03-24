/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Slave.c
 Description:				TWI slave functions

 -------------------------------------------------------
 Program Example
 -------------------------------------------------------
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
					for (i = 0; i < 8; i++)
					{
						TWIS_Write(j++);
					}
					TWIS_Stop();
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
#include <avr/eeprom.h>
#include "twi_slave.h"


uint8_t TWIS_Init()
{
	/*
	** Set the TWI slave address
	*/
#ifdef TWIS_ADDR_EEPROM
	TWAR = eeprom_read_byte(TWIS_ADDR_EEPROM);
#else
	TWAR = TWI_ADDRESS;
#endif
	/*
	** Activate TWI interface
	*/
	TWSR = 0;
	TWBR = 0;
	TWCR = _BV(TWEN) | _BV(TWEA);
	return 1;
}

void TWIS_Stop()
{
	TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

	while (TWCR & _BV(TWINT));

	TWCR = _BV(TWEN) | _BV(TWEA);
}

void TWIS_Write(uint8_t byte)
{
	TWDR = byte;
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);

	while (!(TWCR & _BV(TWINT)));
}

uint8_t TWIS_ReadAck()
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);

	while (!(TWCR & _BV(TWINT)));

	return TWDR;
}

uint8_t TWIS_ReadNack()
{
	TWCR = _BV(TWINT) | _BV(TWEN);

	while (!(TWCR & _BV(TWINT)));

	return TWDR;
}

uint8_t TWIS_ResponseRequired(uint8_t *TWI_ResponseType)
{
	if (TWCR & _BV(TWINT))
	{
		*TWI_ResponseType = TW_STATUS;
		return 1;
	}

	return 0;
}
