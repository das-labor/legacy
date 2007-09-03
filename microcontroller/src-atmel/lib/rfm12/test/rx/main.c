//receive data

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "../../rfm12_hw.h"
#include "../../rfm12.h"

//default fiforeset is as follows:
//2 Byte Sync Pattern,
//disable sensitive reset, Fifo filled interrupt at 8 bits
//default fiforeset register value to accept data
#define ACCEPT_DATA (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4) | RFM12_FIFORESET_FF)
//default fiforeset register value to clear fifo
#define CLEAR_FIFO (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4))



int main()
{
	//some counters
	uint16_t i, rxBytes;
	//spi rx buffer
	uint16_t spiRxBuf = 0;
	
	//init uart
	uart_init();
	
	//init rfm12
	rfm12_init();
	
	sei();
	
	uart_putc('H');
	
	//power up receive circuit (and stay in rx mode)
	rfm12_data(RFM12_CMD_PWRMGT | RFM12_PWRMGT_ER);
	
	//disable sync byte recognition (clears fifo)
//	rfm12_data(CLEAR_FIFO);
	
	//enable sync byte recognition (here we go!)
//	rfm12_data(ACCEPT_DATA);
	
	while(1)
	{
		//disable sync byte recognition (clears fifo)
		rfm12_data(CLEAR_FIFO);
		
		//enable sync byte recognition (here we go!)
		rfm12_data(ACCEPT_DATA);
		
		//data receive loop
		rxBytes = 0;
		while(rxBytes < 8)
		{	
			//wait until data has been received (continuously poll status register)
			do
			{
				
				//save old status register to i (temporarily)
				i = spiRxBuf;
				
				//query status register
				spiRxBuf = rfm12_read(RFM12_CMD_STATUS);
				
				//send status register via uart if its value has changed
				if(i != spiRxBuf)
				{
					PORTC = spiRxBuf >> 6;
					//uart_putc('S');
					//uart_putc(spiRxBuf >> 8);
					//uart_putc(spiRxBuf & 0x00FF);
				}
			}
			while(!(spiRxBuf & RFM12_STATUS_FFIT)); //check bit15, which indicates a FIFO interrupt
			
			//if we're here, data bytes should be ready
			//let's try to read something
			spiRxBuf = rfm12_read(RFM12_CMD_READ) & 0xff;
						
			rxBytes++;
			
			//uart out the received byte
			//hint: the host computer knows that 8 data bytes are coming,
			//as it will also interpret the status register ;)
			uart_putc('D');
					
			uart_putc(spiRxBuf);
		}
		

		//wait some decades
		//for(i=0; i < 65535; i++)
		//{
		//	asm volatile("NOP");
		//}
	}

	return 0;
}
