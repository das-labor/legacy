/**** RFM12 transceiver library **************************************
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  Version 2 as published by the Free Software Foundation.
 *
 *  (c) Soeren Heisrath - soeren at heisrath d0t o r g
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>

#include "rfm12_config.h"
#include "rfm12.h"

void rfm_spi_waitcycle ( void )
{
	uint8_t i;
	for (i=0;i<RFM_SPI_WAITCYCLES;i++)
		asm volatile "nop";
}

inline void rfm_spi_setrx ( void )
{
	RFM_DDR_SPIRX &= ~(RFM_PIN_SPIRX);
	RFM_DDR_SPICLK &= ~(RFM_PIN_SPICLK);
}

inline void rfm_spi_settx ( void )
{
	RFM_DDR_SPITX |= RFM_PIN_SPITX;
	RFM_DDR_SPICLK |= RFM_PIN_SPICLK; // pullup
}

uint16_t rfm_spi_send ( uint16_t in_bytes )
{
	uint16_t mask = 0x0001, dout = 0x0000;

	rfm_spi_settx();
	if (RFM_PORT_SPICLK & RFM_PIN_SPICLK)
	{
		RFM_SPICLK_OFF();
		rfm_spi_waitcycle();
	}

	// check if module's busy.
	if (RFM_PORT_NIRQ & RFM_PIN_NIRQ)
		rfm_spi_waitcycle();
	
	// still busy? return error.
	if (RFM_PORT_NIRQ & RFM_PIN_NIRQ) return 0x0100;

	while (mask)
	{
		if (in_bytes & mask)
		{
			RFM_PORT_SPITX |= RFM_PIN_SPITX;
		} else
		{
			RFM_PORT_SPITX &= ~(RFM_PIN_SPITX);
		}

		RFM_SPICLK_ON();
		rfm_spi_waitcycles();

		if (RFM_PORT_SPIRX & RFM_PIN_SPIRX)
			dout |= 0x0001;

		dout <<= 1;
		mask <<= 1;
		RFM_SPICLK_OFF();
		rfm_spi_waitcycles();
	}
	return dout;
}

/* @brief non-blocking function to receive the contents of the module's
 * fifo buffer.
 *
 * @note returns values > 0xff if nirq was set, meaning module is busy.
 *
 */

uint16_t rfm_spi_receive ( void )
{
	uint16_t tmp = 0x0000;

	if (RFM_PORT_NIRQ & RFM_PIN_NIRQ) return 0xffff;

	rfm_spi_send (0x0000);

	tmp = rfm_spi_send (0xB000);

	rfm_spi_setrx();
	
	return (tmp & 0x00FF);
}

/* @brief set sending mode
 * @note returns values > 0 on error, 0 if successful.
 *
 */

uint8_t rfm_init_tx ( void )
{
	uint16_t tmp = 0x0000;

	tmp |= rfm_spi_send ( 0x80D7 );
	tmp |= rfm_spi_send ( 0x8239 );
	tmp |= rfm_spi_send ( 0xA640 );
	tmp |= rfm_spi_send ( 0xC847 );
	tmp |= rfm_spi_send ( 0xC4A0 );
	tmp |= rfm_spi_send ( 0xC2AC );
	tmp |= rfm_spi_send ( 0xCE81 );
	tmp |= rfm_spi_send ( 0xC483 );
	tmp |= rfm_spi_send ( 0x9850 );
	tmp |= rfm_spi_send ( 0xE000 );
	tmp |= rfm_spi_send ( 0xC800 );
	tmp |= rfm_spi_send ( 0xC040 );
	tmp |= rfm_spi_send ( 0x0000 );
	tmp |= rfm_spi_send ( 0x8239 );

	if (tmp > 0xff) return 0x01; // on error

	return 0x00;
}

/* @note make sure that nIRQ is clear, before sending anything!
 * use rfm_tx_wait instead, unless you know what you're doing.
 */
void rfm_tx ( uint8_t in_byte )
{
	
	rfm_spi_send ( 0xAAAA );
	rfm_spi_send ( 0xAA2D );
	rfm_spi_send ( 0xD400 | in_byte );
}

inline void rfm_tx_wait ( uint8_t in_byte )
{
	while ( RFM_PORT_NIRQ & RFM_PIN_NIRQ )
		asm volatile "nop";
	
	rfm_tx ( in_byte );
}

void rfm_init_rx ( void )
{
	uint16_t tmp = 0x0000;

	tmp |= rfm_spi_send ( 0x80D7 );
	tmp |= rfm_spi_send ( 0x82D9 );
	tmp |= rfm_spi_send ( 0xA640 );
	tmp |= rfm_spi_send ( 0xC847 );
	tmp |= rfm_spi_send ( 0xC4A0 );
	tmp |= rfm_spi_send ( 0xC2AC );
	tmp |= rfm_spi_send ( 0xCE81 );
	tmp |= rfm_spi_send ( 0xC483 );
	tmp |= rfm_spi_send ( 0x9850 );
	tmp |= rfm_spi_send ( 0xE000 );
	tmp |= rfm_spi_send ( 0xC800 );
	tmp |= rfm_spi_send ( 0xC040 );
	tmp |= rfm_spi_send ( 0xCE81 ); // evtl. CA ...
	tmp |= rfm_spi_send ( 0xCE83 ); // ..hier auch

	if (tmp > 0xff) return 0x01;
	return 0x00;}/* @note this one never quits if no data comes in!
 */

uint8_t rfm_rx_wait ( void )
{
	while ( RFM_PORT_NIRQ & RFM_PIN_NIRQ )
		asm volatile "nop";
	
	rfm_spi_send ( 0x0000 );
	rfm_spi_send ( 0xB000 );
}
/* @note non-blocking. result > 0xff means data received.
 */
uint16_t rfm_rx_nb ( void )
{
	if ( ! RFM_PORT_NIRQ & RFM_PIN_NIRQ ) return 0x0000;

	rfm_spi_send ( 0x0000 );
	rfm_spi_send ( 0xB000 );
	rfm_spi_receive (); // first 8 bits are for /dev/null...
	return ( 0x0100 | rfm_spi_receive() );
}
