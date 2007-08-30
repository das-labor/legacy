
#include "rfm12.h"


#define DDR_SS DDRB
#define BIT_SS 

#define SS_ASSERT() PORT_SS |= (1<<BIT_SS)
#define SS_RELEASE() PORT_SS &= ~(1<<BIT_SS)


#define PWRMGT_DEFAULT (RFM12_PWRMGT_DC)

void spi_init(){
	DDR_SPI |= (1<<DD_MOSI) | (1<<DD_SCK);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//SPI Master, clk/16
}


void rfm12_data(uint16_t d){
	while(!(SPSR & (1<<SPIF)));
	SPDR = d>>8;
	while(!(SPSR & (1<<SPIF)));
	SPDR = d & 0xff;
}

void rfm12_init(){
	spi_init();
	DDR_SS |= (1<<BIT_SS);
	SS_RELEASE();
	
	//dissable all power
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
	
	

}
