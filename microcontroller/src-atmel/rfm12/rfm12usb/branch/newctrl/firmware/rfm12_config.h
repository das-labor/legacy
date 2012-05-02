/**** RFM 12 library for Atmel AVR Microcontrollers *******
 * 
 * This software is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * @author Peter Fuhrmann, Hans-Gert Dahmen, Soeren Heisrath
 */

/******************************************************
 *                                                    *
 *           C O N F I G U R A T I O N                *
 *                                                    *
 ******************************************************/

/*
	Connect the RFM12 to the AVR as follows:

	RFM12           | AVR
	----------------+------------
	SDO             | MISO
	nIRQ            | INT1
	FSK/DATA/nFFS   | VCC
	DCLK/CFIL/FFIT  |  -
	CLK             |  -
	nRES            |  -
	GND             | GND
	ANT             |  -
	VDD             | VCC
	GND             | GND
	nINT/VDI        | -
	SDI             | MOSI
	SCK             | SCK
	nSEL            | Slave select pin defined below


*/

#ifndef RFM12_CONFIG_H

#include <avr/io.h>
#include "../common/rfm12_buffer_size.h"
#include "rfmusb_hw.h"


/************************
 * PIN DEFINITIONS
 */


#if HWREF == LABNODE

	//SPI port
	#define DDR_SPI DDRB
	#define PORT_SPI PORTB
	#define PIN_SPI PINB
	#define BIT_MOSI 5
	#define BIT_MISO 6
	#define BIT_SCK  7
	#define BIT_SPI_SS 4
#else

	//SPI port
	#define DDR_SPI DDRB
	#define PORT_SPI PORTB
	#define PIN_SPI PINB
	#define BIT_MOSI 3
	#define BIT_MISO 4
	#define BIT_SCK  5
	#define BIT_SPI_SS 2
	//this is the hardware SS pin of the AVR - it 
	//needs to be set to output for the spi-interface to work 
	//correctly, independently of the CS pin used for the RFM12
#endif



/************************
 * RFM12 CONFIGURATION OPTIONS
 */

//baseband of the module (either RFM12_BAND_433, RFM12_BAND_868 or RFM12_BAND_912)
#define RFM12_BASEBAND RFM12_BAND_433

//center frequency to use (+-125kHz FSK frequency shift)
#define RFM12_FREQUENCY 433175000UL

//Transmit FSK frequency shift in kHz
#define FSK_SHIFT             125000

//use this for datarates >= 2700 Baud
#define DATARATE_VALUE RFM12_DATARATE_CALC_HIGH(9600.0)

//Receive RSSI Threshold
#define RFM12_RSSI_THRESHOLD  RFM12_RXCTRL_RSSI_79

//Receive Filter Bandwidth
#define RFM12_FILTER_BW       RFM12_RXCTRL_BW_400

//Output power relative to maximum (0dB is maximum)
#define RFM12_POWER           RFM12_TXCONF_POWER_0

//Receive LNA gain
#define RFM12_LNA_GAIN        RFM12_RXCTRL_LNA_6

//crystal load capacitance - the frequency can be verified by measuring the
//clock output of RFM12 and comparing to 1MHz.
//11.5pF seems to be o.k. for RFM12, and 10.5pF for RFM12BP, but this may vary.
#define RFM12_XTAL_LOAD       RFM12_XTAL_11_5PF

//TX BUFFER SIZE
#define RFM12_TX_BUFFER_SIZE 64

//RX BUFFER SIZE (there are going to be 2 Buffers of this size for double_buffering)
#define RFM12_RX_BUFFER_SIZE 64


/************************
 * INTERRUPT VECTOR
 * set the name for the interrupt vector here
 */
 
//the interrupt vector
#define RFM12_INT_VECT (INT1_vect)

//the interrupt mask register
#define RFM12_INT_MSK (GICR)

//the interrupt bit in the mask register
#define RFM12_INT_BIT (INT1)

//the interrupt flag register
#define RFM12_INT_FLAG (GIFR)

//the interrupt bit in the flag register
#define RFM12_FLAG_BIT (INTF1)

//setup the interrupt to trigger on negative edge
#define RFM12_INT_SETUP()   (MCUCR |= (1<<ISC11))


/************************
 * FEATURE CONFIGURATION
 */

#define RFM12_LIVECTRL 1
#define RFM12_NORETURNS 0
#define RFM12_NOCOLLISIONDETECTION 0
#define RFM12_TRANSMIT_ONLY 0
#define RFM12_SPI_SOFTWARE 0
#define RFM12_USE_POLLING 0
#define RFM12_RECEIVE_ASK 0
#define RFM12_TRANSMIT_ASK 0
#define RFM12_USE_WAKEUP_TIMER 0
#define RFM12_USE_POWER_CONTROL 0
#define RFM12_LOW_POWER 0
#define RFM12_USE_CLOCK_OUTPUT 0


/************************
 * UART DEBUGGING
 * en- or disable debugging via uart.
 */
 
#define RFM12_UART_DEBUG 0

#define RFM12_CONFIG_H
#endif
