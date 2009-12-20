/* -*- Mode: C; tab-width: 2 -*- */
/*
 * PowerCommander - Ausführungs Kontroller

 diese Version nimmt nur i2c nachrichten an und gibt sie direkt
 aus. Es wird nur dieser input verarbeitet, keine taster oder aehnliches
 ferner gibt es ein paar neue schalter, die das licht im vortragsraum
 zeilenweise (flipper, schraenke, beamer,tafel) ein und ausschalten
 lassen
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "twi_slave/twi_slave.h"

#include "PowerCommander.h"

#include "pc_init.h"

#include "fkt.h"

int main(void)
{
	uint8_t TWIS_ResponseType;
	struct t_i2cproto i2cslave = { C_NDEF, O_NDEF, F_NDEF, HASNDATA, HASNDATA, D_NDEF ,D_NDEF };

	init_modi();
	init_relais();
	init_timer();

	/*
	** Clear any interrupt
	*/
	cli();


	/*
	** Wait 0.5 second for POR
	*/
//	_delay_ms(500);

	/*
	** Start TWI Slave with address 15 and bitrate of 100000 Hz
	*/
	TWIS_Init(I2CADRESSE, 100000);


	/*
		mainloop - die ist die kommunikation mit einem entferntem
		host
	*/
	while (1) 
	{
		if (TWIS_ResponseRequired(&TWIS_ResponseType))
		{
			switch (TWIS_ResponseType)
			{
				/*
				** Slave is requests to read bytes from the master.
				*/
				case TW_SR_SLA_ACK:
				{
					i2cslave.class   = TWIS_ReadAck();
					i2cslave.object  = TWIS_ReadAck();
					i2cslave.fkt     = TWIS_ReadAck();
					i2cslave.in_data = TWIS_ReadNack();
					TWIS_Stop();                // I2C stop
					i2cslave.has_out_data = HASNDATA;

					switch (i2cslave.class)
					{
						case C_SW:
							switch_fkt(&i2cslave);
							break;
						case C_PWM:
							pwm_fkt(&i2cslave);
							break;
						case C_VIRT:
							virt_fkt(&i2cslave);
							break;
						default:
							break;
					}
				}
				break;

				case TW_ST_SLA_ACK:
					if (i2cslave.has_out_data == HASDATA)
						TWIS_Write(i2cslave.out_data);
					else
						TWIS_Write(D_NDEF);
					TWIS_Stop();
					i2cslave.has_out_data = HASNDATA;
					i2cslave.out_data = D_NDEF;
					break;

				default:
					TWIS_Stop();
					break;
			}
		}
	}
}

