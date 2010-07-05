#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <stdlib.h>

#include "pinning.h"
#include "../../common/protocol.h"

#include "rfm12_config.h"
#include "rfm12.h"

volatile uint8_t myaddr = 0x00;

void send_packet (uint8_t in_dst, uint8_t in_cmd, uint8_t in_len, uint8_t *in_data)
{
	uint8_t txbuf[8], i, k=0;

	txbuf[F_T0]  = PROTO_TYPE0;
	txbuf[F_T1]  = PROTO_TYPE1;
	txbuf[F_SRC] = myaddr;
	txbuf[F_DST] = in_dst;
	txbuf[F_CMD] = in_cmd;
	
	for (i=F_DATA;i<(sizeof(txbuf)-PACKET_MINLEN);i++)
	{
		if (i >= in_len + PACKET_MINLEN) break;
		txbuf[i] = in_data[k];
		k++;
	}

	rfm12_tx (in_len + PACKET_MINLEN, RFM12_PROTO_TYPE, txbuf);
}

int main (void)
{
	uint8_t txbuf[2], i, k;

	/* disable ADC & interrupt - save some power */
	ADCSRA &= ~(_BV(ADEN) | _BV(ADIE));

	clock_prescale_set (clock_div_4);
	eeprom_busy_wait();
	myaddr = eeprom_read_byte(0);
	rfm12_init();
	sei();
	_delay_ms(250);
	rfm12_init();
	rfm12_set_batt_detector(0); /* 1.98V */
	
	for (k=0;k<3;k++)
	{
		if (rfm12_get_batt_status() == RFM12_BATT_LOW)
			send_packet (ADDR_BCAST, CMD_LOW_POWER, 0, txbuf);
		else
			send_packet (ADDR_BCAST, CMD_MAIL_SENSOR, 0, txbuf);

		for (i=0;i<0x40;i++)
		{
			_delay_ms(2);
			rfm12_tick();
		}
		_delay_ms(100);
	}

	cli();
	//rfm12_data(0xffff); /* wakeup to max */
	//rfm12_data(0xC800); /* disable low duty cycle */ 
	rfm12_data(0x8201); /* send the rf12 pwrmgmt command to disable all functions of the ic  */
	//rfm12_data(0x8203); /* send the rf12 pwrmgmt command to disable all functions of the ic  */
//	rfm12_data(0x8201); /* send the rf12 pwrmgmt command to disable all functions of the ic  */
//	rfm12_data(0x8203); /* send the rf12 pwrmgmt command to disable all functions of the ic  */
	power_all_disable();
	clock_prescale_set (clock_div_256);
	set_sleep_mode (_BV(SM0) | _BV(SM1)); /* standby */
	sleep_enable();
//	sleep_bod_enable();
	sei();
	sleep_cpu();
	/* ...sleep for ever... */

	while(23);
}
