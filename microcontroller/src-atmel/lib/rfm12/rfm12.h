//rfm12 public header file

//function protoypes
void rfm12_init();
void rfm12_data(uint16_t d);
void rfm12_tx(uint8_t tape, uint8_t length, uint8_t *data);
uint16_t rfm12_read(uint16_t d);

static inline uint8_t rfm12_rx_status();
static inline uint8_t rfm12_tx_status();
static inline uint8_t *rfm12_rx_buffer();
static inline uint8_t rfm12_rx_protocol();
static inline void rfm12_rx_clear();
static inline uint8_t rfm12_rx_buffer_len();


//size of transmit buffer
#define RFM12_TX_BUFFER_SIZE 30

//rx buffer size
#define RFM12_RX_BUFFER_SIZE 30

#define STATUS_FREE 0
#define STATUS_OCCUPIED 1
#define STATUS_COMPLETE 2



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

typedef struct{
	uint8_t status; 		//is the buffer ready or occupied?
	uint8_t num_bytes; 		//number of bytes to be received
	uint8_t bytecount;   //received bytes counter
	
	//rx protocol fields
	uint8_t len;			//length byte - number of bytes in buffer
	uint8_t type;			//type field for airlab
	uint8_t checksum;		//rx checksum
	uint8_t buffer[RFM12_RX_BUFFER_SIZE]; //receive buffer
}rf_rx_buffer_t;

//Buffer and status for the message to be transmitted
extern volatile rf_tx_buffer_t rf_tx_buffer;

//buffer and status for the message to be received
extern volatile rf_rx_buffer_t rf_rx_buffer;

//inline function to return the rx buffer status byte
static inline uint8_t rfm12_rx_status()
{
	return rf_rx_buffer.status;
}

//inline function to return tx buffer status byte
static inline uint8_t rfm12_tx_status()
{
	return rf_tx_buffer.status;
}

//inline function to retrieve current buffer pointer
static inline uint8_t *rfm12_rx_buffer()
{
	return rf_rx_buffer.buffer;
}

//inline function to get protocol type byte
static inline uint8_t rfm12_rx_protocol()
{
	return rf_rx_buffer.type;
}

//inline function to get buffer byte count
static inline uint8_t rfm12_rx_buffer_len()
{
	return rf_rx_buffer.len;
}

//inline function to clear buffer complete/occupied status
static inline void rfm12_rx_clear()
{
	rf_rx_buffer.status = STATUS_FREE;
}
