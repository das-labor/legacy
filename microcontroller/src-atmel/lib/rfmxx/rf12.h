#define RF_PORT	PORTD
#define RF_DDR	DDRD
#define RF_PIN	PIND
#define F_CPU 16000000L

#define SDI		7
#define SCK		6
#define CS		5
#define SDO		3

#define RFM_PIN_NIRQ	(_BV(PD3))
#define RFM_PORT_NIRQ	PIND
#define RFM_DDR_NIRQ	DDRD

extern unsigned short rf12_trans(unsigned short wert);					// transfer 1 word to/from module
extern void rf12_init(void);											// initialize module
extern void rf12_setfreq(unsigned short freq);							// set center frequency
extern void rf12_setbaud(unsigned short baud);							// set baudrate
extern void rf12_setpower(unsigned char power, unsigned char mod);		// set transmission settings
extern void rf12_setbandwidth(unsigned char bandwidth, unsigned char gain, unsigned char drssi);	// set receiver settings
extern void rf12_txdata(uint8_t data, unsigned char number);		// transmit number of bytes from array
extern uint8_t rf12_rxdata(void);		// receive number of bytes into array
extern void rf12_ready(void);											// wait until FIFO ready (to transmit/read data)

#define RF12FREQ(freq)	((freq-430.0)/0.0025)							// macro for calculating frequency value out of frequency in MHz

