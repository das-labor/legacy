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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "../rfm12_config.h"
#include "../spi.h"
#include "rfm12_hw.h"
#include "rfm12.h"

#if RFM12_UART_DEBUG
	#include "test/uart.h"
#endif

//evil hack
#ifdef RFM12_TRANSMIT_ONLY
#undef RFM12_PWRMGT_ER
#define RFM12_PWRMGT_ER 0
#endif


//default value for powermanagement register
//here it can be selected, if we want the reset generator or the
//oscillator or the wakeup timer to be active
#ifndef PWRMGT_DEFAULT
#define PWRMGT_DEFAULT (RFM12_PWRMGT_DC)
#endif

//default preamble (altrernating 1s and 0s)
#define PREAMBLE 0xAA

//default synchron pattern
#define SYNC_MSB 0x2D
#define SYNC_LSB 0xD4

/*
	Begin of library
*/

#define SS_ASSERT() PORT_SS &= ~(1<<BIT_SS)
#define SS_RELEASE() PORT_SS |= (1<<BIT_SS)

#define RFM12_INT_ON() RFM12_INT_MSK |= (1<<RFM12_INT_BIT)
#define RFM12_INT_OFF() RFM12_INT_MSK &= ~(1<<RFM12_INT_BIT)


//default fiforeset is as follows:
//2 Byte Sync Pattern,
//disable sensitive reset, Fifo filled interrupt at 8 bits
//default fiforeset register value to accept data
#define ACCEPT_DATA (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4) | RFM12_FIFORESET_FF)
#define ACCEPT_DATA_INLINE (RFM12_FIFORESET_DR | (8<<4) | RFM12_FIFORESET_FF)
//default fiforeset register value to clear fifo
#define CLEAR_FIFO (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4))
#define CLEAR_FIFO_INLINE (RFM12_FIFORESET_DR | (8<<4))


//Buffer and status for the message to be transmitted
rf_tx_buffer_t rf_tx_buffer;

//buffer and status for the message to be received
rf_rx_buffer_t rf_rx_buffer;

//mode we are in - rx or tx
#define MODE_RX 0
#define MODE_TX 1
uint8_t rfm12_mode;

/* @description Actual sending function to send raw data to the Module
 * @note do NOT call this function directly, unless you know what you're doing.
 */

/*
void spi_init()
{
	DDR_SPI |= (1<<BIT_MOSI) | (1<<BIT_SCK) | (1<<BIT_SPI_SS);
#ifndef SPI_SOFTWARE	
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//SPI Master, clk/16
#endif
} */

void rfm12_data(uint16_t d)
{
	SS_ASSERT();
	spi_send(d>>8);
	spi_send(d & 0xff);
	SS_RELEASE();
}


uint16_t rfm12_read(uint16_t c)
{
	uint16_t retval;
	SS_ASSERT();
	
	retval = spi_send(c>>8)<<8;
	retval |= spi_send(c & 0xff);
	
	SS_RELEASE();
	return retval;
}


/* @description reads the upper 8 bits of the status
 * register (the interrupt flags)
 */
static inline uint8_t rfm12_read_int_flags_inline()
{
	SS_ASSERT();

	SPDR = 0;
	while(!(SPSR & (1<<SPIF)));
	SS_RELEASE();
	return SPDR;
}

/* @description inline version of rfm12_data for use in interrupt
 */
static inline void rfm12_data_inline(uint8_t cmd, uint8_t d)
{
	SS_ASSERT();

	SPDR = cmd;
	while(!(SPSR & (1<<SPIF)));

	SPDR = d;
	while(!(SPSR & (1<<SPIF)));

	SS_RELEASE();
}

/* @description inline function for reading the fifo
 */
static inline uint8_t rfm12_read_fifo_inline()
{
	SS_ASSERT();

	SPDR =  ( RFM12_CMD_READ >> 8 );
	while(!(SPSR & (1<<SPIF)));

	SPDR = 0;
	while(!(SPSR & (1<<SPIF)));

	SS_RELEASE();
	return SPDR;
}


/*
	interrupt handler for interrupts from rfm12
	
	The receiver will generate an interrupt request (IT) for the
	microcontroller - by pulling the nIRQ pin low - on the following events:

	* The TX register is ready to receive the next byte (RGIT)
	* The FIFO has received the preprogrammed amount of bits (FFIT)
	* Power-on reset (POR) *
	* FIFO overflow (FFOV) / TX register underrun (RGUR) *
	* Wake-up timer timeout (WKUP) *
	* Negative pulse on the interrupt input pin nINT (EXT) *
	* Supply voltage below the preprogrammed value is detected (LBD) *
	*these shouldn't occur - we'll just ignore them. These flags are cleared
	 by reading status.
*/


ISR(RFM12_INT_VECT, ISR_NOBLOCK){
	RFM12_INT_OFF();
	uint8_t status;

	//first we read the first byte of the status register
	//to get the interrupt flags
	status = rfm12_read_int_flags_inline();

#ifdef RFM12_USE_WAKEUP_TIMER
	if(status & (RFM12_STATUS_WKUP>>8)){
		
		//FIXME: crude aproach of using rfm12_mode to decide what to write to 
		//PWRMGT register.
		//should be changed to using a shadow var for the PWRMGT state
		if(rfm12_mode == MODE_RX){
			rfm12_data(RFM12_CMD_PWRMGT | (PWRMGT_DEFAULT & ~RFM12_PWRMGT_EW) | RFM12_PWRMGT_ER );
			rfm12_data(RFM12_CMD_PWRMGT |  PWRMGT_DEFAULT                     | RFM12_PWRMGT_ER );
		}else{
			rfm12_data(RFM12_CMD_PWRMGT | (PWRMGT_DEFAULT & ~RFM12_PWRMGT_EW) | RFM12_PWRMGT_ET );
			rfm12_data(RFM12_CMD_PWRMGT |  PWRMGT_DEFAULT                     | RFM12_PWRMGT_ET );
		}
	}
#endif
	
	//check if the fifo interrupt occurred
	if(status & (RFM12_STATUS_FFIT>>8))
	{

#if RFM12_UART_DEBUG >= 2
			uart_putc('F');
#endif
		if (rfm12_mode == MODE_TX)
		{
			//the fifo interrupt occurred, and we are in TX mode,
			//so the fifo wants the next byte to TX.

#if RFM12_UART_DEBUG >= 2
			uart_putc('T');
#endif
			
			if(rf_tx_buffer.bytecount < rf_tx_buffer.num_bytes)
			{
				//load the next byte from our buffer struct.
				rfm12_data_inline( (RFM12_CMD_TX>>8), rf_tx_buffer.sync[rf_tx_buffer.bytecount++]);
			} else
			{
				//o.k. - so we are finished transmitting the bytes
				
				//flag the buffer as free again
				rf_tx_buffer.status = STATUS_FREE;
				
				//we are now in rx mode again
				rfm12_mode = MODE_RX;
				
				//turn off the transmitter
				//and enable receiver
				rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
				
				//load a dummy byte to clear int status.
				rfm12_data_inline( (RFM12_CMD_TX>>8), 0xaa);
				
				//init receiver fifo
				rfm12_data_inline(RFM12_CMD_FIFORESET>>8, CLEAR_FIFO_INLINE);
				rfm12_data_inline(RFM12_CMD_FIFORESET>>8, ACCEPT_DATA_INLINE);
			}
		} else
		{
			static uint8_t checksum;

#if RFM12_UART_DEBUG >= 2
			uart_putc('R');
			uart_putc(rfm12_read_fifo_inline());
#endif
		
			/* check if we're receiving a new transmission or the next byte of
			 * an active transmission
			 *
			 * FIXME: this could cause problems, b/c we could be ignoring a
			 * transmission if the buffer is in STATUS_COMPLETE
			 *
			 * FIXME: this in turn could lead to a false sync byte recognition
			 * in the middle of a transfer
			 * 
			 * FIXME: we should silently clock in the transmission without
			 * writing it to the buffer OR apply double buffering
			 */
			if(rf_rx_buffer.status == STATUS_IDLE)
			{
				rf_rx_buffer.bytecount = 1;

				//read first byte, which is the length byte
				checksum = rfm12_read_fifo_inline();
				rf_rx_buffer.num_bytes = checksum + 3;
				
				if(rf_rx_buffer.rf_buffer_in->status == STATUS_FREE){
					//the current receive buffer is empty, so we start receiving
					rf_rx_buffer.status = STATUS_RECEIVING;
				
					rf_rx_buffer.rf_buffer_in->len = checksum;//hackhack: checksum holds length
				}else{
					//the buffer is full, so we ignore this transmission.
					rf_rx_buffer.status = STATUS_IGNORING;
				}
				
			}
			//transfer is active, continue receipt
			else if(rf_rx_buffer.status == STATUS_RECEIVING)
			{
				//check if transmission is complete
				if(rf_rx_buffer.bytecount < rf_rx_buffer.num_bytes)
				{
					uint8_t data;
					data = rfm12_read_fifo_inline();
					checksum ^= data;	
					
					//put next byte into buffer, if there is enough space
					if(rf_rx_buffer.bytecount < (RFM12_RX_BUFFER_SIZE + 3))
					{
						//hackhack: begin writing to struct at offsetof len
						(& rf_rx_buffer.rf_buffer_in->len)[rf_rx_buffer.bytecount] = data;
					}
					
					//check crc
					if((rf_rx_buffer.bytecount == 2) && (checksum != 0xff) )
					{
						//invalid crc, reset transmission
						rf_rx_buffer.status = STATUS_IDLE;
					
						//reset fifo
						rfm12_data_inline(RFM12_CMD_FIFORESET>>8, CLEAR_FIFO_INLINE);
						rfm12_data_inline(RFM12_CMD_FIFORESET>>8, ACCEPT_DATA_INLINE);
					}

					//increment bytecount
					rf_rx_buffer.bytecount++;
				}
				//transmission is complete, reset fifo
				else
				{
					uint8_t num;
					//indicate that the receiver is idle again
					rf_rx_buffer.status = STATUS_IDLE;
					
					//indicate that the Buffer is ready to be used
					rf_rx_buffer.rf_buffer_in->status = STATUS_COMPLETE;
					
					//switch to other buffer
					num = rf_rx_buffer.buffer_in_num;
					num = (num+1) % 2;
					rf_rx_buffer.buffer_in_num = num;

					rf_rx_buffer.rf_buffer_in = &rf_rx_buffer.rf_buffers[num];
					//reset fifo
					rfm12_data_inline(RFM12_CMD_FIFORESET>>8, CLEAR_FIFO_INLINE);
					rfm12_data_inline(RFM12_CMD_FIFORESET>>8, ACCEPT_DATA_INLINE);
				}
			} else if (rf_rx_buffer.status == STATUS_IGNORING)
			{
				//FIXME: we don't check the checksum if we ignore a package.
				//that could mean that we ignore a wrong number of bytes, but it shouldn't
				//hurt in practice
				
				//check if transmission is complete
				if(rf_rx_buffer.bytecount < rf_rx_buffer.num_bytes)
				{
					//remove byte from fifo so interrupt condition is cleared.
					rfm12_read_fifo_inline();
				}else
				{
					//indicate that the receiver is idle again
					rf_rx_buffer.status = STATUS_IDLE;
					//reset fifo
					rfm12_data_inline(RFM12_CMD_FIFORESET>>8, CLEAR_FIFO_INLINE);
					rfm12_data_inline(RFM12_CMD_FIFORESET>>8, ACCEPT_DATA_INLINE);	
				}
			}
		}
	}

	RFM12_INT_ON();
}


void rfm12_tick()
{
	uint16_t status;
	static uint8_t channel_free_count = 16;

#if RFM12_UART_DEBUG
	static uint8_t oldmode;
	uint8_t mode = rfm12_mode;
	if (oldmode != mode)
	{
		uart_putstr ("mode change: ");
		switch (mode)
		{
			case MODE_RX:
				uart_putc ('r');
			break;
			case MODE_TX:
				uart_putc ('t');
			break;
			default:
				uart_putc ('?');
		}
		oldmode = mode;
	}
#endif

	//don't disturb RFM12 if transmitting
	if(rfm12_mode == MODE_TX)
		return;

	//disable the interrupt (as we're working directly with the transceiver now)
	RFM12_INT_OFF();
		
	status = rfm12_read(RFM12_CMD_STATUS);
	
	RFM12_INT_ON();
	
	//check if we see a carrier
	if(status & RFM12_STATUS_RSSI){
		//yes: reset free counter
		channel_free_count = 200;
	}else{
		//no: decrement counter
		channel_free_count--;
		//is the channel free long enough ?
		if(channel_free_count == 0){
			channel_free_count = 1;

			//do we have something to transmit?
			if(rf_tx_buffer.status == STATUS_OCCUPIED)
			{
				//yes: start transmitting
				
				//disable the interrupt (as we're working directly with the transceiver now)
				RFM12_INT_OFF();
				
				//disable receiver - is this needed?
				rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);
			
				//set mode for interrupt handler
				rfm12_mode = MODE_TX;
				
				//fill 2byte 0xAA preamble into data register
				//this is explicitly done, just to be sure
				//(hint: the tx FIFO [if el is enabled] is two staged, so we can safely write 2 bytes before starting)
				rfm12_data(RFM12_CMD_TX | PREAMBLE);
				rfm12_data(RFM12_CMD_TX | PREAMBLE);
				
				//set ET in power register to enable transmission
				//(hint: TX starts now)
				rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ET);
			
				RFM12_INT_ON();
			}
		}
	}

}

#ifdef RFM12_RECEIVE_CW

rfrxbuf_t cw_rxbuf;

ISR(ADC_vect, ISR_NOBLOCK){
	static uint16_t adc_average;
	static uint8_t pulse_timer;
	
	       uint8_t    value;
	static uint8_t oldvalue;
	static uint8_t ignore;
	uint16_t adc;
	

	
	ADCSRA = (1<<ADEN) | (1<<ADFR) | (0<<ADIE) //start free running mode
			| (1<<ADPS2) | (1<<ADPS1);  //preescaler to clk/64
										//samplerate = 16MHz/(64*13) = 19231 Hz

	
	adc = ADC;
	
	adc_average -= adc_average/64;
	adc_average +=adc;
	
	value = (ADC > ((adc_average/64)+50) )?1:0;
	
	if(value){
		PORTD |= (1<<PD7);
	}else{
		PORTD &= ~(1<<PD7);
	}


	if(TCNT0 > 0xE0){
		ignore = 0;
	}
	
	if(cw_rxbuf.state == STATE_EMPTY){
		if(value && (!ignore) ){
			//pulse_timer = 0;
			TCNT0 = 0;
			cw_rxbuf.p   = 0;
			cw_rxbuf.state = STATE_RECEIVING;
		}
	}else if(cw_rxbuf.state == STATE_FULL){
		if(value){
			TCNT0 = 0;
			ignore = 1;
		}
		
	}else if(cw_rxbuf.state == STATE_RECEIVING){
		if(value != oldvalue){

			cw_rxbuf.buf[cw_rxbuf.p] = TCNT0;
			TCNT0 = 0;
			//pulse_timer = 0;
			if(cw_rxbuf.p != (RFRXBUF_SIZE-1) ) cw_rxbuf.p++;
		}else if(TCNT0 > 0xe0){
			//if( !value ){
			//PORTD |= (1<<PD6);
				cw_rxbuf.state = STATE_FULL;
			//}else{
			//	cw_rxbuf.state = STATE_EMPTY;
			//}
		}
	}

	oldvalue = value;

	ADCSRA = (1<<ADEN) | (1<<ADFR) | (1<<ADIE) //start free running mode
			| (1<<ADPS2) | (1<<ADPS1);  //preescaler to clk/64
										//samplerate = 16MHz/(64*13) = 19231 Hz


}

void adc_init(){
	ADMUX  = (1<<REFS0) | (1<<REFS1); //Internal 2.56V Reference, MUX0
	
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADIE) //start free running mode
			| (1<<ADPS2) | (1<<ADPS1);  //preescaler to clk/64
										//samplerate = 16MHz/(64*13) = 19231 Hz
	
}

#endif


/* @description ask the rfm12 to transmit a packet when possible (carrier sense)
 * the data should be written to the tx buffer first after asking if
 * it is empty.
 *
 * @return see rfm12.h for possible return values.
 */
uint8_t rfm12_start_tx(uint8_t type, uint8_t length)
{
	//exit if the buffer isn't free
	if(rf_tx_buffer.status != STATUS_FREE)
		return RFM12_TX_OCCUPIED;
	
	//calculate number of bytes to be sent by ISR
	//2 sync bytes + len byte + type byte + checksum + message length + 1 dummy byte
	rf_tx_buffer.num_bytes = length + 6;
	
	//write airlab header to buffer
	rf_tx_buffer.len = length;
	rf_tx_buffer.type = type;
	rf_tx_buffer.checksum = length ^ type ^ 0xff;
	
	//reset counter
	rf_tx_buffer.bytecount = 0;
	
	//schedule packet for transmission
	rf_tx_buffer.status = STATUS_OCCUPIED;
	
	return RFM12_TX_ENQUEUED;
}

/* @description send data out
 * @return see rfm12.h for possible return values.
 */
uint8_t rfm12_tx ( uint8_t len, uint8_t type, uint8_t *data )
{
	#if RFM12_UART_DEBUG
		uart_putstr ("sending: ");
		uart_putstr ((char*)data);
		uart_putstr ("\r\n");
	#endif
	if (len > RFM12_TX_BUFFER_SIZE) return RFM12_TX_ERROR;

	//exit if the buffer isn't free
	if(rf_tx_buffer.status != STATUS_FREE)
		return RFM12_TX_OCCUPIED;
	
	memcpy ( rf_tx_buffer.buffer, data, len );
	return rfm12_start_tx (type, len);
}





void rfm12_init()
{
	SS_RELEASE();
	//DDR_SS |= (1<<BIT_SS);
	
	spi_init();

	rf_tx_buffer.sync[0] = SYNC_MSB;
	rf_tx_buffer.sync[1] = SYNC_LSB;
	
	rf_rx_buffer.rf_buffer_out = &rf_rx_buffer.rf_buffers[0];
	rf_rx_buffer.rf_buffer_in  = &rf_rx_buffer.rf_buffers[0];
	
	//disable all power
	//this was no good idea, because when one writes the PWRMGT register
	//two times in a short time, the second write seems to be delayed.
	//the PWRMGT register is written at the end of this function.
	//rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);

	//enable internal data register and fifo
	rfm12_data(RFM12_CMD_CFG | RFM12_CFG_EL | RFM12_CFG_EF | RFM12_BAND_433 | RFM12_XTAL_12PF);
	
	//automatic clock lock control(AL), digital Filter(!S), Data quality
	//detector value 3
	rfm12_data(RFM12_CMD_DATAFILTER | RFM12_DATAFILTER_AL | 3);
	
	//2 Byte Sync Pattern, Start fifo fill when sychron pattern received,
	//disable sensitive reset, Fifo filled interrupt at 8 bits
	rfm12_data(RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4));

	//set AFC to automatic, (+4 or -3)*2.5kHz Limit, fine mode, active and enabled
	rfm12_data(RFM12_CMD_AFC | RFM12_AFC_AUTO_KEEP | RFM12_AFC_LIMIT_4
				| RFM12_AFC_FI | RFM12_AFC_OE | RFM12_AFC_EN);
	
	//set frequency
	rfm12_data(RFM12_CMD_FREQUENCY | RFM12_FREQUENCY_CALC_433(FREQ) );

	//set datarate
	rfm12_data(RFM12_CMD_DATARATE | DATARATE_VALUE );

	//set rx parameters: int-in/vdi-out pin is vdi-out,
	//Bandwith, LNA, RSSI
	rfm12_data(RFM12_CMD_RXCTRL | RFM12_RXCTRL_P16_VDI 
			| RFM12_RXCTRL_VDI_FAST | RFM12_RXCTRL_BW_400 | RFM12_RXCTRL_LNA_6 
			| RFM12_RXCTRL_RSSI_79 );
	
	//set TX Power to -0dB, frequency shift = +-125kHz
	rfm12_data(RFM12_CMD_TXCONF | RFM12_TXCONF_POWER_0 | RFM12_TXCONF_FS_CALC(125000) );
	
	//enable receiver
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
		
	//init receiver fifo, we now begin receiving.
	rfm12_data(CLEAR_FIFO);
	rfm12_data(ACCEPT_DATA);

	//setup interrupt for falling edge trigger
	RFM12_INT_SETUP();
	
	//activate the interrupt
	RFM12_INT_ON();

#ifdef RFM12_RECEIVE_CW
	adc_init();
#endif
}


#ifdef RFM12_USE_WAKEUP_TIMER
void rfm12_set_wakeup_timer(uint16_t val){
	rfm12_data (RFM12_CMD_WAKEUP | val);
}
#endif

#ifdef RFM12_LOW_POWER
void rfm12_powerDown()
{
	RFM12_INT_OFF();
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);	
}

uint8_t rfm12_lowPowerTx( uint8_t len, uint8_t type, uint8_t *data )
{
	uint8_t retVal;
	
	//enable whole receiver chain
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
	
	//tick once (reactivates rfm12 int)
	rfm12_tick();
	
	//tx
	retVal = rfm12_tx(len, type, data);
	
	//tx packet
	while(rfm12_mode != RFM12_TX)	
	{
		rfm12_tick();
	}
	
	//wait for tx complete
	while(rf_tx_buffer.status != STATUS_FREE);
	
	//powerdown rfm12
	rfm12_powerDown();
	
	return retVal;
}
#endif
