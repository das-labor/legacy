//rfm12 public header file
//author peter+hansi

//function protoypes
void rfm12_init();
void rfm12_data(uint16_t d);
void rfm12_tx(uint8_t tape, uint8_t length, uint8_t *data);
uint16_t rfm12_read(uint16_t d);

static inline uint8_t rfm12_tx_status();
static inline void rfm12_rx_clear();

//size of transmit buffer
#define RFM12_TX_BUFFER_SIZE 30

//rx buffer size
//there are going to be 2 Buffers of this size
//(double_buffering)
#define RFM12_RX_BUFFER_SIZE 30

#define STATUS_FREE 0
#define STATUS_OCCUPIED 1
#define STATUS_COMPLETE 2



/* Private structs needed for inline functions */
/* DO NOT MODIFY ANY SOURCE BELOW HERE ;-) */


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
	uint8_t status; 		//is the buffer ready or occupied?
	uint8_t len;			//length byte - number of bytes in buffer
	uint8_t type;			//type field for airlab
	uint8_t checksum;		//rx checksum
	uint8_t buffer[RFM12_RX_BUFFER_SIZE]; //receive buffer
}rf_buffer_t;


typedef struct{
	uint8_t num_bytes;		//number of bytes to be received
	uint8_t bytecount;		//received bytes counter
	
	//points to the rf_buffer in rf_buffers that will be filled next
	rf_buffer_t * rf_buffer_in;
	//points to the rf_buffer in rf_buffers that will be read next
	rf_buffer_t * rf_buffer_out;
	
}rf_rx_buffer_t;

//this prototype needs to be declared after the struct declaration...
static inline rf_buffer_t * rfm12_rx_buffer();


extern volatile rf_buffer_t rf_buffers[2];

//Buffer and status for the message to be transmitted
extern volatile rf_tx_buffer_t rf_tx_buffer;

//buffer and status for the message to be received
extern volatile rf_rx_buffer_t rf_rx_buffer;

//inline function to return the rx buffer status byte
//this function works on the output buffer
static inline uint8_t rfm12_rx_status()
{
	return rf_rx_buffer.rf_buffer_out->status;
}

//inline function to retrieve current rf_buffer struct pointer
//this function works on the output buffer
static inline rf_buffer_t * rfm12_rx_buffer()
{
	return rf_rx_buffer.rf_buffer_out;
}

//inline function to clear buffer complete/occupied status
//this function works on the output buffer
static inline void rfm12_rx_clear()
{
	//note: no buffer flipping here if the interrupt is receiving data
	//BUT... if the in buffer is already complete, then we need to flip the buffers
	//otherwise the interrupt does the job
	
	rf_rx_buffer.rf_buffer_out->status = STATUS_FREE;
	
	//flip buffers if necessary
	if(rf_rx_buffer.rf_buffer_in->status == STATUS_COMPLETE)
	{
		rf_buffer_t * tmpBuf = rf_rx_buffer.rf_buffer_in;
		rf_rx_buffer.rf_buffer_in = rf_rx_buffer.rf_buffer_out;
		rf_rx_buffer.rf_buffer_out = tmpBuf;
	}
}
