//author peter+hansi

#include <avr/io.h>
#include <avr/interrupt.h>

#include "rfm12_hw.h"
#include "rfm12.h"


/*
	Set User Parameters here
*/

//Pin that slave select is connected to
#define DDR_SS DDRB
#define PORT_SS PORTB
#define BIT_SS 4

//SPI port
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define PIN_SPI PINB
#define BIT_MOSI 5
#define BIT_MISO 6
#define BIT_SCK  7


//frequency to use
#define FREQ 433000000UL

//use this for datarates >= 2700 Baud
#define DATARATE_VALUE RFM12_DATARATE_CALC_HIGH(9600.0)

//use this for 340 Baud < datarate < 2700 Baud
//#define DATARATE_VALUE RFM12_DATARATE_CALC_LOW(1200.0)


//default value for powermanagement register
//her it can be selected, if we want the reset generator or the
//oscillator or the wakeup timer to be active
#define PWRMGT_DEFAULT (RFM12_PWRMGT_DC)

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


//Buffer and status for the message to be transmitted
volatile rf_buffer_t rf_tx_buffer;


/*
	interrupt handler for interrupts from rfm12
	
	The receiver will generate an interrupt request (IT) for the microcontroller - by pulling the nIRQ pin low - on the following events:
	• The TX register is ready to receive the next byte (RGIT)
	• The FIFO has received the preprogrammed amount of bits (FFIT)
	• Power-on reset (POR) *
	• FIFO overflow (FFOV) / TX register underrun (RGUR) *
	• Wake-up timer timeout (WKUP) *
	• Negative pulse on the interrupt input pin nINT (EXT) *
	• Supply voltage below the preprogrammed value is detected (LBD) *
	*these shouldn't occur - we'll just ignore them.
*/
ISR(INT0_vect){
	uint8_t status;
	
	//first we read the first byte of the status register
	//to get the interrupt flags
	SS_ASSERT();
	SPDR = 0;
	while(!(SPSR & (1<<SPIF)));
	SS_RELEASE();
	status = SPDR;
	
	//check which interrupt occurred
	if()
}


void spi_init(){
	DDR_SPI |= (1<<BIT_MOSI) | (1<<BIT_SCK);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//SPI Master, clk/16
}


void rfm12_data(uint16_t d){
	SS_ASSERT();

	SPDR = d>>8;
	while(!(SPSR & (1<<SPIF)));

	SPDR = d & 0xff;
	while(!(SPSR & (1<<SPIF)));
	
	SS_RELEASE();
}

uint16_t rfm12_read(uint16_t d)
{
	uint16_t retval;
	SS_ASSERT();
	
	SPDR = d>>8;
	while(!(SPSR & (1<<SPIF)));
	retval = SPDR<<8;
	SPDR = d & 0xff;
	while(!(SPSR & (1<<SPIF)));
	retval |= SPDR;
	
	SS_RELEASE();
	return retval;
}

void rfm12_init(){
	spi_init();
	DDR_SS |= (1<<BIT_SS);
	SS_RELEASE();
	
	//disable all power
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);

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
}

//send arbitrary length data with preamble and synchron pattern
void rfm12_tx(uint8_t length, uint8_t *data)
{
	uint8_t txBytes;
	
	//fill 2byte 0xAA preamble into data register
	//this is explicitly done, just to be sure
	//(hint: the tx FIFO [if el is enabled] is two staged, so we can safely write 2 bytes before starting)
	rfm12_data(RFM12_CMD_TX | PREAMBLE);
	rfm12_data(RFM12_CMD_TX | PREAMBLE);
	
	//set ET in power register to enable transmission
	//(hint: TX starts now)
	rfm12_data(RFM12_CMD_PWRMGT | RFM12_PWRMGT_ET);
	
	
	//monitor SDO pin to see if TX FIFO is ready to accept the next byte
	SS_ASSERT();
	while(!(PIN_SPI & (1<<BIT_MISO)));
	
	//fill fifo with first synchron pattern byte
	rfm12_data(RFM12_CMD_TX | SYNC_MSB);
	
	//second sync byte
	SS_ASSERT();
	while(!(PIN_SPI & (1<<BIT_MISO)));
	rfm12_data(RFM12_CMD_TX | SYNC_LSB);
	
	//enter data loop and baller out data
	for(txBytes = 0; txBytes < length; txBytes++)
	{
		SS_ASSERT();
		while(!(PIN_SPI & (1<<BIT_MISO)));
		rfm12_data(RFM12_CMD_TX | data[txBytes]);
	}
	
	//send a dummy byte to flush the last data byte into the next FIFO stage
	SS_ASSERT();
	while(!(PIN_SPI & (1<<BIT_MISO)));
	rfm12_data(RFM12_CMD_TX | PREAMBLE);
	
	//wait again and turn off the transceiver
	SS_ASSERT();
	while(!(PIN_SPI & (1<<BIT_MISO)));
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);
}
