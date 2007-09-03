//rfm12 public header file


#define RFM12_TX_BUFFER_SIZE 30

typedef struct{
	uint8_t status; 		//are we transmitting, or is the buffer free?
	uint8_t len; 			//number of bytes to transmit
	uint8_t _sync[2]; 		//sync bytes - so we can transmit beginning here
	uint8_t _tlen, _itlen;	//length byte an inverted length byte so
							//receiver knows how many bytes to expect.
	uint8_t buffer[RFM12_TX_BUFFER_SIZE];//buffer for the raw bytes to be transmitted
}rf_buffer_t;

extern volatile rf_buffer_t rf_tx_buffer;


//function protoypes
void rfm12_init();
void rfm12_data(uint16_t d);
void rfm12_tx(uint8_t length, uint8_t *data);
uint16_t rfm12_read(uint16_t d);
