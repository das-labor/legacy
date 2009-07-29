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

/************************
 * standard includes
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/************************
 * library internal includes
 * the order in which they are included is important
*/
#include "include/rfm12_hw.h"
#include "include/rfm12_core.h"
#include "rfm12.h"

//for uart debugging
#if RFM12_UART_DEBUG
	#include "test-m8/uart.h"
#endif


/************************
 * library internal globals
*/

//buffer and status for the message to be transmitted
rf_tx_buffer_t rf_tx_buffer;

//if receive mode is not disabled (default)
#if !(RFM12_TRANSMIT_ONLY)
	//buffers for the message to be received
	rf_rx_buffer_t rf_rx_buffers[2];
#endif /* RFM12_USE_WAKEUP_TIMER */

//the control struct
rfm12_control_t ctrl;


/************************
 * load other core and external components
 * (putting them directly into here allows the GCC to optimize better)
*/

/* include spi functions into here */
#include "include/rfm12_spi.c"

/*
 * include control / init functions into here
 * all of the stuff in there is optional, so there's no code-bloat..
*/
#include "include/rfm12_ctrl.c"

/*
 * include extra features here
 * all of the stuff in there is optional, so there's no code-bloat..
*/
#include "include/rfm12_extra.c"


/************************
 * Begin of library
*/

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
ISR(RFM12_INT_VECT, ISR_NOBLOCK)
{
	RFM12_INT_OFF();
	uint8_t status;
	
	//if receive mode is not disabled (default)
	#if !(RFM12_TRANSMIT_ONLY)
		static uint8_t checksum; //static local variables produce smaller code size than globals
	#endif /* !(RFM12_TRANSMIT_ONLY) */

	//first we read the first byte of the status register
	//to get the interrupt flags
	status = rfm12_read_int_flags_inline();

	//low battery detector feature
	#if RFM12_LOW_BATT_DETECTOR
	if(status & (RFM12_STATUS_LBD>>8))
	{
		//debug
		#if RFM12_UART_DEBUG >= 2
			uart_putc('L');
		#endif
		
		//set status variable to low battery
		ctrl.low_batt = RFM12_BATT_LOW;
	}
	#endif /* RFM12_LOW_BATT_DETECTOR */	
	
	//wakeup timer feature
	#if RFM12_USE_WAKEUP_TIMER
	if(status & (RFM12_STATUS_WKUP>>8))
	{
		//debug
		#if RFM12_UART_DEBUG >= 2
			uart_putc('W');
		#endif
		
		//restart the wakeup timer by toggling the bit on and off
		rfm12_data(ctrl.pwrmgt_shadow & ~RFM12_PWRMGT_EW);
		rfm12_data(ctrl.pwrmgt_shadow);
	}
	#endif /* RFM12_USE_WAKEUP_TIMER */
	
	//check if the fifo interrupt occurred
	if((!status & (RFM12_STATUS_FFIT>>8)))
		goto END;
	
	//see what we have to do (start rx, rx or tx)
	switch(ctrl.rfm12_state)
	{			
		case STATE_RX_IDLE:
			//if receive mode is not disabled (default)
			#if !(RFM12_TRANSMIT_ONLY)
				//init the bytecounter - remember, we will read the length byte, so this must be 1
				ctrl.bytecount = 1;

				//read the length byte,  and write it to the checksum
				//remember, the first byte is the length byte			
				checksum = rfm12_read_fifo_inline();
				
				//add the packet overhead and store it into a working variable
				ctrl.num_bytes = checksum + PACKET_OVERHEAD;
				
				//debug
				#if RFM12_UART_DEBUG >= 2
					uart_putc('I');
					uart_putc(checksum);
				#endif
				
				//see whether our buffer is free
				//FIXME: put this into global statekeeping struct, the free state can be set by the function which pulls the packet, i guess
				if(ctrl.rf_buffer_in->status == STATUS_FREE)
				{
					//the current receive buffer is empty, so we start receiving
					ctrl.rfm12_state = STATE_RX_ACTIVE;
				
					//store the received length into the packet buffer
					//FIXME:  why the hell do we need this?!
					//in principle, the length is stored alongside with the buffer.. the only problem is, that the buffer might be cleared during reception
					ctrl.rf_buffer_in->len = checksum;
					
					//end the interrupt without resetting the fifo
					goto END;
				}
			
				/* if we're here, the buffer is full, so we ignore this transmission by resetting the fifo (at the end of the function)  */
			#endif /* !(RFM12_TRANSMIT_ONLY) */
			break;
			
		case STATE_RX_ACTIVE:
			//if receive mode is not disabled (default)
			#if !(RFM12_TRANSMIT_ONLY)
				//check if transmission is complete
				if(ctrl.bytecount < ctrl.num_bytes)
				{
					uint8_t data;
					
					//read a byte
					data = rfm12_read_fifo_inline();
					
					//debug
					#if RFM12_UART_DEBUG >= 2
						uart_putc('R');
						uart_putc(data);
					#endif
					
					//xor the remaining bytes onto the checksum
					//note: only the header will be effectively checked
					checksum ^= data;
					
					//put next byte into buffer, if there is enough space
					if(ctrl.bytecount < (RFM12_RX_BUFFER_SIZE + 3))
					{
						//hackhack: begin writing to struct at offsetof len
						(& ctrl.rf_buffer_in->len)[ctrl.bytecount] = data;
					}
					
					//check header against checksum
					if (ctrl.bytecount == 2 && checksum != 0xff)
					{
						//if the checksum does not match, reset the fifo
						break;
					}

					//increment bytecount
					ctrl.bytecount++;
					
					//end the interrupt without resetting the fifo
					goto END;
				}
				
				/* if we're here, receiving is done */
				/* the fifo will be reset at the end of the function */

				//debug
				#if RFM12_UART_DEBUG >= 2
					uart_putc('D');
				#endif
				
				//indicate that the buffer is ready to be used
				ctrl.rf_buffer_in->status = STATUS_COMPLETE;
				
				//switch to other buffer
				ctrl.buffer_in_num = (ctrl.buffer_in_num + 1) % 2;
				ctrl.rf_buffer_in = &rf_rx_buffers[ctrl.buffer_in_num];
			#endif /* !(RFM12_TRANSMIT_ONLY) */
			break;
			
		case STATE_TX:
			//debug
			#if RFM12_UART_DEBUG >= 2
				uart_putc('T');
			#endif

			if(ctrl.bytecount < ctrl.num_bytes)
			{
				//load the next byte from our buffer struct.
				rfm12_data_inline( (RFM12_CMD_TX>>8), rf_tx_buffer.sync[ctrl.bytecount++]);
							
				//end the interrupt without resetting the fifo
				goto END;
			}
			
			/* if we're here, we're finished transmitting the bytes */
			/* the fifo will be reset at the end of the function */
			
			//flag the buffer as free again
			ctrl.txstate = STATUS_FREE;
			
			//if receive mode is not disabled (default)
			#if !(RFM12_TRANSMIT_ONLY)
				//wakeup timer feature				
				#if RFM12_USE_WAKEUP_TIMER
					//clear wakeup timer once
					rfm12_data(ctrl.pwrmgt_shadow & ~RFM12_PWRMGT_EW);					
					//write shadow register
					ctrl.pwrmgt_shadow = (RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);							
				#endif /* RFM12_USE_WAKEUP_TIMER */
				
				//turn off the transmitter and enable receiver
				rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
			#else
				//wakeup timer feature
				#if RFM12_USE_WAKEUP_TIMER
					//clear timer once
					rfm12_data(ctrl.pwrmgt_shadow & ~RFM12_PWRMGT_EW);
					//write shadow register
					ctrl.pwrmgt_shadow = (RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);
				#endif /* RFM12_USE_WAKEUP_TIMER */	
				
				//turn off the transmitter only
				rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);				
			#endif /* !(RFM12_TRANSMIT_ONLY) */
			
			//load a dummy byte to clear int status
			rfm12_data_inline( (RFM12_CMD_TX>>8), 0xaa);
			break;			
	}
	
	//set the state machine to idle
	ctrl.rfm12_state = STATE_RX_IDLE;
	
	//reset the receiver fifo, if receive mode is not disabled (default)
	#if !(RFM12_TRANSMIT_ONLY)
		rfm12_data_inline(RFM12_CMD_FIFORESET>>8, CLEAR_FIFO_INLINE);
		rfm12_data_inline(RFM12_CMD_FIFORESET>>8, ACCEPT_DATA_INLINE);
	#endif /* !(RFM12_TRANSMIT_ONLY) */	
		
	END:
	//turn the int back on
	RFM12_INT_ON();
}


void rfm12_tick()
{	
	//collision detection is enabled by default
	#if !(RFM12_NOCOLLISIONDETECTION)
		uint16_t status;
		
		//start with a channel free count of 16, this is necessary for the CW (ADC)  receive feature to work
		static uint8_t channel_free_count = 16; //static local variables produce smaller code size than globals
	#endif

	//debug
	#if RFM12_UART_DEBUG
		static uint8_t oldstate;
		uint8_t state = ctrl.rfm12_state;
		if (oldstate != state)
		{
			uart_putstr ("mode change: ");
			switch (state)
			{
				case STATE_RX_IDLE:
					uart_putc ('i');
					break;
				case STATE_RX_ACTIVE:
					uart_putc ('r');
					break;
				case STATE_TX:
					uart_putc ('t');
					break;
				default:
					uart_putc ('?');
			}
			uart_putstr ("\r\n");
			oldstate = state;
		}
	#endif
	
	//don't disturb RFM12 if transmitting or receiving
	//FIXME: raw tx mode is excluded from this check now
	if(ctrl.rfm12_state != STATE_RX_IDLE)
	{
		return;
	}	
		
	//collision detection is enabled by default
	#if !(RFM12_NOCOLLISIONDETECTION)
		//disable the interrupt (as we're working directly with the transceiver now)
		//hint: we could be losing an interrupt here 
		//solutions: check status flag if int is set, launch int and exit ... OR implement packet retransmission
		RFM12_INT_OFF();	
		status = rfm12_read(RFM12_CMD_STATUS);
		RFM12_INT_ON();

		//check if we see a carrier
		if(status & RFM12_STATUS_RSSI)
		{
			//yes: reset free counter and return
			channel_free_count = CHANNEL_FREE_TIME;
			return;
		}
		
		//no: decrement counter
		channel_free_count--;
		
		//is the channel free long enough ?
		if(channel_free_count != 0)
		{
			return;
		}
		
		//reset the channel free count for the next decrement (during the next call..)
		channel_free_count = 1;
	#endif	
	
	//do we have something to transmit?
	if(ctrl.txstate == STATUS_OCCUPIED)
	{ //yes: start transmitting
		//disable the interrupt (as we're working directly with the transceiver now)
		//hint: we could be losing an interrupt here, too
		//we could also disturb an ongoing reception,
		//if it just started some cpu cycles ago 
		//(as the check for this case is some lines (cpu cycles) above)
		//anyhow, we MUST transmit at some point...
		RFM12_INT_OFF();
		
		//disable receiver - if you don't do this, tx packets will get lost
		//as the fifo seems to be in use by the receiver
		rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);
		
		//calculate number of bytes to be sent by ISR
		//2 sync bytes + len byte + type byte + checksum + message length + 1 dummy byte
		ctrl.num_bytes = rf_tx_buffer.len + 6;
		
		//reset byte sent counter
		ctrl.bytecount = 0;
		
		//set mode for interrupt handler
		ctrl.rfm12_state = STATE_TX;
		
		//wakeup timer feature
		#if RFM12_USE_WAKEUP_TIMER		
			ctrl.pwrmgt_shadow = (RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ET);
		#endif /* RFM12_USE_WAKEUP_TIMER */
		
		//fill 2byte 0xAA preamble into data register
		//the preamble helps the receivers AFC circuit to lock onto the exact frequency
		//(hint: the tx FIFO [if el is enabled] is two staged, so we can safely write 2 bytes before starting)
		rfm12_data(RFM12_CMD_TX | PREAMBLE);
		rfm12_data(RFM12_CMD_TX | PREAMBLE);
		
		//set ET in power register to enable transmission (hint: TX starts now)
		rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ET);

		//enable the interrupt to continue the transmission
		RFM12_INT_ON();
	}

}


/* @description ask the rfm12 to transmit a packet when possible (carrier sense)
 * the data should be written to the tx buffer first after asking if
 * it is empty.
 *
 * @return see rfm12.h for possible return values.
 */
#if (RFM12_NORETURNS)
void 
#else
uint8_t
#endif
rfm12_start_tx(uint8_t type, uint8_t length)
{
	//exit if the buffer isn't free
	if(ctrl.txstate != STATUS_FREE)
		return TXRETURN(RFM12_TX_OCCUPIED);
	
	//write airlab header to buffer
	rf_tx_buffer.len = length;
	rf_tx_buffer.type = type;
	rf_tx_buffer.checksum = length ^ type ^ 0xff;
	
	//schedule packet for transmission
	ctrl.txstate = STATUS_OCCUPIED;
	
	return TXRETURN(RFM12_TX_ENQUEUED);
}

/* @description send data out
 * @return see rfm12.h for possible return values.
 */
#if (RFM12_NORETURNS)
void
#else
uint8_t 
#endif
rfm12_tx ( uint8_t len, uint8_t type, uint8_t *data )
{
	#if RFM12_UART_DEBUG
		uart_putstr ("sending packet\r\n");
	#endif
	
	if (len > RFM12_TX_BUFFER_SIZE) return TXRETURN(RFM12_TX_ERROR);

	//exit if the buffer isn't free
	if(ctrl.txstate != STATUS_FREE)
		return TXRETURN(RFM12_TX_OCCUPIED);
	
	memcpy ( rf_tx_buffer.buffer, data, len );

	#if (!(RFM12_NORETURNS))
	return rfm12_start_tx (type, len);
	#else
	rfm12_start_tx (type, len);
	#endif
}


//if receive mode is not disabled (default)
#if !(RFM12_TRANSMIT_ONLY)
	//function to clear buffer complete/occupied status
	//warning: without the attribute, gcc will inline this even if -Os is set
	void __attribute__ ((noinline)) void rfm12_rx_clear()
	{
			//mark the current buffer as empty
			ctrl.rf_buffer_out->status = STATUS_FREE;
			
			//switch to the other buffer
			ctrl.buffer_out_num = (ctrl.buffer_out_num + 1 ) % 2 ;
			ctrl.rf_buffer_out = &rf_rx_buffers[ctrl.buffer_out_num];
		
	}
#endif /* !(RFM12_TRANSMIT_ONLY) */


//main library initialization function
void rfm12_init()
{
	//initialize spi
	SS_RELEASE();
	DDR_SS |= (1<<BIT_SS);	
	spi_init();

	//enable internal data register and fifo
	//setup selected band
	rfm12_data(RFM12_CMD_CFG | RFM12_CFG_EL | RFM12_CFG_EF | RFM12_BAND_433 | RFM12_XTAL_12PF);
	
	//set power default state (usually disable clock output)
	//do not write the power register two times in a short time
	//as it seems to need some recovery
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);

	//set frequency
	rfm12_data(RFM12_CMD_FREQUENCY | RFM12_FREQUENCY_CALC_433(FREQ) );

	//set datarate
	rfm12_data(RFM12_CMD_DATARATE | DATARATE_VALUE );
	
	//set rx parameters: int-in/vdi-out pin is vdi-out,
	//Bandwith, LNA, RSSI
	rfm12_data(RFM12_CMD_RXCTRL | RFM12_RXCTRL_P16_VDI 
			| RFM12_RXCTRL_VDI_FAST | RFM12_RXCTRL_BW_400 | RFM12_RXCTRL_LNA_6 
			| RFM12_RXCTRL_RSSI_79 );	
	
	//automatic clock lock control(AL), digital Filter(!S),
	//Data quality detector value 3, slow clock recovery lock
	rfm12_data(RFM12_CMD_DATAFILTER | RFM12_DATAFILTER_AL | 3);
	
	//2 Byte Sync Pattern, Start fifo fill when sychron pattern received,
	//disable sensitive reset, Fifo filled interrupt at 8 bits
	rfm12_data(RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4));

	//set AFC to automatic, (+4 or -3)*2.5kHz Limit, fine mode, active and enabled
	rfm12_data(RFM12_CMD_AFC | RFM12_AFC_AUTO_KEEP | RFM12_AFC_LIMIT_4
				| RFM12_AFC_FI | RFM12_AFC_OE | RFM12_AFC_EN);
	
	//set TX Power to -0dB, frequency shift = +-125kHz
	rfm12_data(RFM12_CMD_TXCONF | RFM12_TXCONF_POWER_0 | RFM12_TXCONF_FS_CALC(125000) );
	
	//disable low dutycycle mode
	rfm12_data(RFM12_CMD_DUTYCYCLE);
	
	//disable wakeup timer
	rfm12_data(RFM12_CMD_WAKEUP);

	//store the syncronization pattern to the transmission buffer
	//the sync pattern is used by the receiver to distinguish noise from real transmissions
	//the sync pattern is hardcoded into the receiver
	rf_tx_buffer.sync[0] = SYNC_MSB;
	rf_tx_buffer.sync[1] = SYNC_LSB;
	
	//if receive mode is not disabled (default)
	#if !(RFM12_TRANSMIT_ONLY)
		//init buffer pointers
		ctrl.rf_buffer_out = &rf_rx_buffers[0];
		ctrl.rf_buffer_in  = &rf_rx_buffers[0];
		//ctrl.buffer_in_num = 0;
		//ctrl.buffer_out_num = 0;
	#endif /* !(RFM12_TRANSMIT_ONLY) */
	
	//raw tx feature initialization
	#if RFM12_RAW_TX
		rfm12_raw_tx = 0;
	#endif
	
	//low battery detector feature initialization
	#if RFM12_LOW_BATT_DETECTOR
		ctrl.low_batt = RFM12_BATT_OKAY;
	#endif /* RFM12_LOW_BATT_DETECTOR */
	
	//enable rf receiver chain, if receiving is not disabled (default)
	#if !(RFM12_TRANSMIT_ONLY)
		rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
	#endif /* !(RFM12_TRANSMIT_ONLY) */
	
	//wakeup timer feature setup
	#if RFM12_USE_WAKEUP_TIMER
		//if receive mode is not disabled (default)
		#if !(RFM12_TRANSMIT_ONLY)
			//set power management shadow register to receiver chain enabled
			ctrl.pwrmgt_shadow = (RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);			
		#else
			//set power management shadow register to power off
			ctrl.pwrmgt_shadow = (RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);
		#endif /* !(RFM12_TRANSMIT_ONLY) */
	#endif /* RFM12_USE_WAKEUP_TIMER */
	
	//raw receive mode feature initialization
	#if RFM12_RECEIVE_CW
		adc_init();
	#endif

	//setup interrupt for falling edge trigger
	RFM12_INT_SETUP();
	
	//clear int flag
	rfm12_read(RFM12_CMD_STATUS);
	RFM12_INT_FLAG |= (1<<RFM12_FLAG_BIT);		
	
	//init receiver fifo, we now begin receiving.
	rfm12_data(CLEAR_FIFO);
	rfm12_data(ACCEPT_DATA);
	
	//activate the interrupt
	RFM12_INT_ON();	
}

