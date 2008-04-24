/**** RFM 12 library for Atmel AVR Microcontrollers *******
 *
 * This software is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * @author Peter Fuhrmann, Hansi (insert realname here), Soeren Heisrath
 */

/******************************************************
 *                                                    *
 *           C O N F I G U R A T I O N                *
 *                                                    *
 ******************************************************/


/**** TX ERROR HANDLING
 * en- or disable error handling for sending data
 * 0 = disable, >=1 enable
 *
 * this option enables return states for rfm12_tx() and it
 * adds slightly more overhead. This option is only recommended
 * and probably only useful for cases where you have multiple
 * entities/functions sending out data via the rfm12 module.
 */
#define RFM12_ENABLETXERRORHANDLING 0

/**** TX BUFFER SIZE
 */
#define RFM12_TX_BUFFER_SIZE 30

/**** RX BUFFER SIZE
 * there are going to be 2 Buffers of this size
 * (double_buffering)
 */
#define RFM12_RX_BUFFER_SIZE 30

/**** UART DEBUGGING
 * en- or disable debugging via uart.
 */
#define RFM12_UART_DEBUG 1

/**** INTERRUPT VECTOR
 * set the name for the interrupt vector here
 */
#define RFM12_INT_VECT (INT0_vect)

/******************************************************
 *                                                    *
 *     E N D   O F   C O N F I G U R A T I O N        *
 *                                                    *
 *      ( thou shalt not change lines below )         *
 *                                                    *
 ******************************************************/

//states for the rx and tx buffers
#define STATUS_FREE 0
#define STATUS_OCCUPIED 1
#define STATUS_COMPLETE 2

//states for the rx state machine
#define STATUS_IDLE      0
#define STATUS_RECEIVING 1
#define STATUS_IGNORING  2

// possible return values for rfm12_tx() and
// rfm12_start_tx()
#define RFM12_TX_SUCCESS 0x00

#define RFM12_TX_OVERFLOW 0x01
#define RFM12_TX_ERROR 0x02
#define RFM12_TX_OCCUPIED 0x03

#define RFM12_TX_ENQUEUED 0x80




//function protoypes
void rfm12_init();
void rfm12_data(uint16_t d);

void rfm12_tick();

uint8_t rfm12_start_tx(uint8_t type, uint8_t length);

uint8_t rfm12_tx(uint8_t len, uint8_t type, uint8_t *data);

uint16_t rfm12_read(uint16_t d);

//static inline uint8_t rfm12_tx_status();

static inline uint8_t *rfm12_rx_buffer();
static inline void rfm12_rx_clear();


/* Private structs needed for inline functions */

typedef struct{
	uint8_t status; 		//is the buffer free or occupied?
	uint8_t num_bytes; 		//number of bytes to transmit (size in buffer)
	uint8_t bytecount;   //counter for the byte we are transmitting at the moment
	
	//we transmit the bytes beginning here
	uint8_t sync[2]; 		//sync bytes for receiver to start filling fifo
	uint8_t len;			//length byte - number of bytes in buffer
	uint8_t type;			//type field for airlab
	uint8_t checksum;		//checksum over the former 2
	uint8_t buffer[RFM12_TX_BUFFER_SIZE];//buffer for the raw bytes to be transmitted
}rf_tx_buffer_t;


//for storing the received bytes.
typedef struct{
	uint8_t status; 		//is the buffer free or is there complete data in it?
	uint8_t len;			//length byte - number of bytes in buffer
	uint8_t type;			//type field for airlab
	uint8_t checksum;		//rx checksum
	uint8_t buffer[RFM12_RX_BUFFER_SIZE]; //receive buffer
}rf_buffer_t;


typedef struct{
	uint8_t status;			//are we idle, receiving or ignoring?
	uint8_t num_bytes;		//number of bytes to be received
	uint8_t bytecount;		//received bytes counter
	
	//Buffers for storing incoming transmissions
	rf_buffer_t rf_buffers[2];
	
	//points to the rf_buffer in rf_buffers that will be filled next
	rf_buffer_t * rf_buffer_in;
	uint8_t buffer_in_num;
	
	//points to the rf_buffer in rf_buffers that will be retruned for reading next
	rf_buffer_t * rf_buffer_out;
	uint8_t buffer_out_num;
}rf_rx_buffer_t;


//Buffer and status for the message to be transmitted
extern volatile rf_tx_buffer_t rf_tx_buffer;

//buffer and status for the message to be received
extern volatile rf_rx_buffer_t rf_rx_buffer;


//inline function to return the rx buffer status byte
//(returns STATUS_FREE or STATUS_COMPLETE)
static inline uint8_t rfm12_rx_status()
{
	return rf_rx_buffer.rf_buffer_out->status;
}

static inline uint8_t rfm12_rx_len()
{
	return rf_rx_buffer.rf_buffer_out->len;
}

static inline uint8_t rfm12_rx_type()
{
	return rf_rx_buffer.rf_buffer_out->type;
}



//inline function to retrieve current rf buffer
static inline uint8_t *rfm12_rx_buffer()
{
	return rf_rx_buffer.rf_buffer_out->buffer;
}



//inline function to clear buffer complete/occupied status
static inline void rfm12_rx_clear()
{
	//mark the current buffer as empty
	rf_rx_buffer.rf_buffer_out->status = STATUS_FREE;
	
	//switch to the other buffer
	rf_rx_buffer.buffer_out_num = (rf_rx_buffer.buffer_out_num + 1 ) % 2 ;
	rf_rx_buffer.rf_buffer_out = &rf_rx_buffer.rf_buffers[rf_rx_buffer.buffer_out_num];
}
