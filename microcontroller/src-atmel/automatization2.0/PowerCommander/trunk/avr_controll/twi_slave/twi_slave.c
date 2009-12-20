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

				case TWIS_ReadBytes:
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

				case TWIS_WriteBytes:
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

/****************************************************************************
  TWI State codes
****************************************************************************/
// General TWI Master staus codes                      
#define TWI_START					0x08  // START has been transmitted  
#define TWI_REP_START				0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST				0x38  // Arbitration lost

// TWI Master Transmitter staus codes                      
#define TWI_MTX_ADR_ACK				0x18  // SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK			0x20  // SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK			0x28  // Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK			0x30  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver staus codes  
#define TWI_MRX_ADR_ACK				0x40  // SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK			0x48  // SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK			0x50  // Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK			0x58  // Data byte has been received and NACK tramsmitted

// TWI Slave Transmitter staus codes
#define TWI_STX_ADR_ACK				0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST	0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK			0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK			0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE	0xC8  // Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received

// TWI Slave Receiver staus codes
#define TWI_SRX_ADR_ACK				0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST	0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK				0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST	0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK		0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK		0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK		0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK		0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART		0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
#define TWI_NO_STATE				0xF8  // No relevant state information available; TWINT = “0”
#define TWI_BUS_ERROR				0x00  // Bus error due to an illegal START or STOP condition

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
	TWBR = ((F_CPU/Bitrate)-16)/2;
	if (TWBR < 11) return 0;
/*
** Set the TWI slave address
*/
	TWAR = (Address << 1);
/*
** Activate TWI interface
*/
	TWCR = (1<<TWEN)|(1<<TWEA);

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
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO)|(1<<TWEA);
}
/*******************************************************
 Public Function: TWIS_Write

 Purpose: Write a byte to the master

 Input Parameter:
 	- uint8_t	Byte to be sent

 Return Value: uint8_t
  	- TRUE:		OK, Byte sent
 	- FALSE:	Error in byte transmission

*******************************************************/
void TWIS_Write(uint8_t byte)
{
	TWDR = byte;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
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
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
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
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
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
uint8_t	TWIS_ResponseRequired(uint8_t *TWI_ResponseType)
{
	*TWI_ResponseType = TWSR;
	return TWCR & (1<<TWINT);
}

