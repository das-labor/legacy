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

#include "include/rfm12_hw.h"
#include "rfm12.h"
#include "include/rfm12_core.h"

//for uart debugging
#if RFM12_UART_DEBUG
	#include "test/uart.h"
#endif

/*
	Begin of library
*/


#if (RFM12_NORETURNS)
	#define TXRETURN(a)
#else
	#define TXRETURN(a) (a)
#endif

//Buffer and status for the message to be transmitted
rf_tx_buffer_t rf_tx_buffer;

//buffer and status for the message to be received
rf_rx_buffer_t rf_rx_buffer;

//these are the states for the receive/transmit state machine
#define STATE_RX_IDLE 0
#define STATE_RX_ACTIVE 1
#define STATE_TX 2
//FIXME: raw mode...
#define MODE_RAW 2
uint8_t rfm12_state;

/* include spi functions into here */
#include "include/rfm12_spi.c"

/* include control / init functions into here */
#include "rfm12_ctrl.c"

/* include extra features here */
#include "rfm12_extra.c"


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
	static uint8_t checksum;

	//first we read the first byte of the status register
	//to get the interrupt flags
	status = rfm12_read_int_flags_inline();

	#if RFM12_USE_WAKEUP_TIMER > 0
		if(status & (RFM12_STATUS_WKUP>>8))
		{
			
			//FIXME: crude aproach of using rfm12_mode to decide what to write to 
			//PWRMGT register.
			//should be changed to using a shadow var for the PWRMGT state
			if(rfm12_mode == MODE_RX)
			{
				rfm12_data(RFM12_CMD_PWRMGT | (PWRMGT_DEFAULT & ~RFM12_PWRMGT_EW) | RFM12_PWRMGT_ER );
				rfm12_data(RFM12_CMD_PWRMGT |  PWRMGT_DEFAULT                     | RFM12_PWRMGT_ER );
			}else
			{
				rfm12_data(RFM12_CMD_PWRMGT | (PWRMGT_DEFAULT & ~RFM12_PWRMGT_EW) | RFM12_PWRMGT_ET );
				rfm12_data(RFM12_CMD_PWRMGT |  PWRMGT_DEFAULT                     | RFM12_PWRMGT_ET );
			}
		}
	#endif
	
	//check if the fifo interrupt occurred
	if((!status & (RFM12_STATUS_FFIT>>8)))
		goto END;
	
	//debug
	#if RFM12_UART_DEBUG >= 2
		uart_putc('F');
	#endif		
	
	switch(rfm12_state)
	{
		case STATE_TX:
			//debug
			#if RFM12_UART_DEBUG >= 2
			uart_putc('T');
			#endif

			if(rf_tx_buffer.bytecount < rf_tx_buffer.num_bytes)
			{
				//load the next byte from our buffer struct.
				rfm12_data_inline( (RFM12_CMD_TX>>8), rf_tx_buffer.sync[rf_tx_buffer.bytecount++]);
				
				//end the interrupt without resetting the fifo
				goto END;
			}
			
			/* if we're here, we're finished transmitting the bytes */
			/* the fifo will be resetted at the end of the function */
			
			//flag the buffer as free again
			rf_tx_buffer.status = STATUS_FREE;
			
			//we are now accepting transmissions again
			rfm12_state = STATE_RX_IDLE;
			
			//turn off the transmitter
			//and enable receiver
			rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
			
			//load a dummy byte to clear int status
			rfm12_data_inline( (RFM12_CMD_TX>>8), 0xaa);			
			break;
			
		case STATE_RX_IDLE:		
			rf_rx_buffer.bytecount = 1;

			//read the length byte,  and write it to the checksum
			//remember, the first byte is the length byte			
			checksum = rfm12_read_fifo_inline();
			
			//add the packet overhead and store it into a working variable
			//FIXME: make the overhead a define
			rf_rx_buffer.num_bytes = checksum + 3;
			
			//debug
			#if RFM12_UART_DEBUG >= 2
			uart_putc('I');
			uart_putc(rfm12_read_fifo_inline());
			#endif
			
			//see whether our buffer is free
			//FIXME: put this into global statekeeping struct, the free state can be set by the function which pulls the packet, i guess
			if(rf_rx_buffer.rf_buffer_in->status == STATUS_FREE)
			{
				//the current receive buffer is empty, so we start receiving
				rfm12_state = STATE_RX_ACTIVE;
			
				//FIXME:  why the hell do we need this?!
				//in principle, the length is stored alongside with the buffer.. the only problem is, that the buffer might be cleared during reception
				rf_rx_buffer.rf_buffer_in->len = rf_rx_buffer.num_bytes;
				
				//end the interrupt without resetting the fifo
				goto END;
			}
			
			/* if we're here, the buffer is full, so we ignore this transmission by resetting the fifo (at the end of the function)  */
			break;
			
		case STATE_RX_ACTIVE:			
			//check if transmission is complete
			if(rf_rx_buffer.bytecount < rf_rx_buffer.num_bytes)
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
				if(rf_rx_buffer.bytecount < (RFM12_RX_BUFFER_SIZE + 3))
				{
					//hackhack: begin writing to struct at offsetof len
					(& rf_rx_buffer.rf_buffer_in->len)[rf_rx_buffer.bytecount] = data;
				}
				
				//check header against checksum
				if (rf_rx_buffer.bytecount == 2 && checksum != 0xff)
				{
					//if the checksum does not match, reset the fifo
					break;
				}

				//increment bytecount
				rf_rx_buffer.bytecount++;
				
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
			rf_rx_buffer.rf_buffer_in->status = STATUS_COMPLETE;
			
			//switch to other buffer
			rf_rx_buffer.buffer_in_num = (rf_rx_buffer.buffer_in_num + 1) % 2;
			rf_rx_buffer.rf_buffer_in = &rf_rx_buffer.rf_buffers[rf_rx_buffer.buffer_in_num];		
			break;
	}
	
	//reset the fifo and resets the state machine to idle
	rfm12_state = STATE_RX_IDLE;
	rfm12_data_inline(RFM12_CMD_FIFORESET>>8, CLEAR_FIFO_INLINE);
	rfm12_data_inline(RFM12_CMD_FIFORESET>>8, ACCEPT_DATA_INLINE);
		
	END:
	RFM12_INT_ON();
}


void rfm12_tick()
{
	uint16_t status;
	#if !(RFM12_NOCOLLISIONDETECTION)
	//FIXME: define the value
	static uint8_t channel_free_count = 16;
	#endif

	//debug
	#if RFM12_UART_DEBUG
	static uint8_t oldstate;
	uint8_t state = rfm12_state;
	if (oldstate != state)
	{
		uart_putstr ("mode change: ");
		switch (mode)
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
		oldstate = state;
	}
	#endif

	//don't disturb RFM12 if trasnmitting or receiving
	//FIXME: raw tx mode is excluded from this check now
	if(rfm12_state != STATE_RX_IDLE)
	{
		return;
	}

	#if !(RFM12_NOCOLLISIONDETECTION)
	//disable the interrupt (as we're working directly with the transceiver now)
	//FIXME: we could be losing an interrupt here (check status flag if int is set, launch int and exit ...
	RFM12_INT_OFF();	
	status = rfm12_read(RFM12_CMD_STATUS);
	RFM12_INT_ON();

	//check if we see a carrier
	if(status & RFM12_STATUS_RSSI)
	{
		//yes: reset free counter and return
		//FIXME: define the value
		channel_free_count = 200;
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
	if(rf_tx_buffer.status == STATUS_OCCUPIED)
	{ //yes: start transmitting
		//disable the interrupt (as we're working directly with the transceiver now)
		//FIXME: again, we need to read the status byte here and see if an int occured, if yes, abort the whole operation and start receiving
		RFM12_INT_OFF();
		
		//set mode for interrupt handler
		rfm12_state = STATE_TX;
		
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
	if(rf_tx_buffer.status != STATUS_FREE)
		return TXRETURN(RFM12_TX_OCCUPIED);
	
	//calculate number of bytes to be sent by ISR
	//2 sync bytes + len byte + type byte + checksum + message length + 1 dummy byte
	rf_tx_buffer.num_bytes = length + 6;
	
	//reset byte sent counter
	rf_tx_buffer.bytecount = 0;
	
	//write airlab header to buffer
	rf_tx_buffer.len = length;
	rf_tx_buffer.type = type;
	rf_tx_buffer.checksum = length ^ type ^ 0xff;
	
	//schedule packet for transmission
	rf_tx_buffer.status = STATUS_OCCUPIED;
	
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
		uart_putstr ("sending: ");
		uart_putstr ((char*)data);
		uart_putstr ("\r\n");
	#endif
	if (len > RFM12_TX_BUFFER_SIZE) return TXRETURN(RFM12_TX_ERROR);

	//exit if the buffer isn't free
	if(rf_tx_buffer.status != STATUS_FREE)
		return TXRETURN(RFM12_TX_OCCUPIED);
	
	memcpy ( rf_tx_buffer.buffer, data, len );

#if (!(RFM12_NORETURNS))
	return rfm12_start_tx (type, len);
#else
	rfm12_start_tx (type, len);
#endif
}


