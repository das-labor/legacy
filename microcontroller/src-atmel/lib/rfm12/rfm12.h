//rfm12 public header file

//function protoypes
void rfm12_init();
void rfm12_data(uint16_t d);
void rfm12_tx(uint8_t tape, uint8_t length, uint8_t *data);
uint16_t rfm12_read(uint16_t d);

static inline uint8_t rfm12_tx_status();

static inline uint8_t *rfm12_rx_buffer();
static inline void rfm12_rx_clear();


//size of transmit buffer
#define RFM12_TX_BUFFER_SIZE 30

//rx buffer size
//there are going to be 2 Buffers of this size
//(double_buffering)
#define RFM12_RX_BUFFER_SIZE 30

//states for the rx and tx buffers
#define STATUS_FREE 0
#define STATUS_OCCUPIED 1
#define STATUS_COMPLETE 2

//states for the rx state machine
#define STATUS_IDLE      0
#define STATUS_RECEIVING 1
#define STATUS_IGNORING  2


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
