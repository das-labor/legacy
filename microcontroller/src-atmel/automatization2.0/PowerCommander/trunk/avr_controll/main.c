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
#include <avr/wdt.h>

#include "twi_slave/twi_slave.h"
#include "PowerCommander.h"
#include "pc_init.h"
#include "fkt.h"

int main(void)
{
	uint8_t TWIS_ResponseType;

	t_outputdata outputdata = {0, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
	init_modi();
	init_io_ports();
	init_timer();
	set_outputs();
	/*
	** Clear any interrupt
	*/
	cli();

	/*
	** Start TWI Slave with address 15 and bitrate of 100000 Hz
	*/
	TWIS_Init();


	/*
		mainloop - die ist die kommunikation mit einem master
	*/
	while (1)
	{
		if (TWIS_ResponseRequired(&TWIS_ResponseType))
		{
			switch (TWIS_ResponseType)
			{
				/*
				** Slave is requested to read bytes from the master.
				*/
				case TW_SR_SLA_ACK:
				{
					outputdata.ports = TWIS_ReadAck();
					outputdata.ports += TWIS_ReadAck() << 8;
					uint8_t i;
					for (i = 0; i < 5; i++)
						outputdata.pwmval[i] = TWIS_ReadAck();
					outputdata.pwmval[i] = TWIS_ReadNack();
					TWIS_Stop();                // I2C stop
					set_outputs();
				}
				break;

				case TW_ST_SLA_ACK:
					TWIS_Write(outputdata.ports);
					TWIS_Write(outputdata.ports >> 8);
					for (int read_p = 0; read_p < 6; read_p++)
						TWIS_Write(outputdata.pwmval[read_p]);
					TWIS_Stop();
					break;

				default:
					TWIS_Stop();
					break;
			}
		}
		wdt_reset();
	}
}

