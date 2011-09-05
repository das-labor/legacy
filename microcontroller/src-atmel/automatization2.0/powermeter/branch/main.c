#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "can_handler.h"
#include "can/spi.h"
#include "can/can.h"
#include "adc_driver.h"
#include "rtc_driver.h"
#include "tc_driver.h"
#include "dma_driver.h"
#include "ursartC1_driver.h"
#include "tests.h"

extern uint16_t DMA_ADC0;
extern uint16_t DMA_ADC1;
extern uint16_t DMA_ADC2;


void sync_osc() {
	
	/*32 MHz Oszillator starten */
	OSC.CTRL |= OSC_RC32MEN_bm;
	
	/*Wenn Oscillator stabil wird das Flag RC32MRDY
	 * gesetzt und 32Mhz können benutzt werden*/
	while (!(OSC.STATUS & OSC_RC32MRDY_bm));

	/* auto kalibierung ein */
//	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

	/* CLK Bus prescaler */
   /* n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
      CLK_PSADIV_1_gc | CLK_PSBCDIV_1_2_gc;
	
    CCP=CCP_IOREG_gc;
    CLK.PSCTRL=n;*/

	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	/*Clock auf 32Mhz einstellen*/
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

void start_mcp_clock(){
	//init the timmer for mcp2515 clock
	PORTD.DIRSET = (1<<2);
	TCD0.CTRLB = TC0_CCCEN_bm | 3; //single slope pwm, OC0C as output
	TCD0.PER = 1;
	TCD0.CCC = 1;
	TCD0.CNT = 0;
	TCD0.CTRLA = 1; //clk/1
}


#define PORT_GRAPH 0x36

void send_graph(uint8_t id, uint16_t * data, uint16_t len){
	static can_message msg = {0, 0, PORT_GRAPH, PORT_GRAPH, 4, {}};
	can_message * txmsg;
	
	msg.addr_src = myaddr;
	msg.dlc = 4;
	msg.data[0] = 0;   //counter (0=start message)
	msg.data[1] = id; 
	msg.data[2] = len;
	msg.data[3] = len>>8;

	
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message));
	can_transmit(txmsg);
	
	msg.dlc = 8;

	uint16_t x;
	for(x=0;x<len;x+=4){
		txmsg = can_buffer_get();
		memcpy(txmsg, &msg, sizeof(can_message) );//header
		memcpy(txmsg->data, &data[x], 8); //data
		can_transmit(txmsg);
	}

}

void Interrupt_Init(void)
{
	//enable ROUND ROBIN !!!!
	uint8_t tmp= PMIC_RREN_bm|PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = tmp; 

	sei();
}

int main(void)
{
	PORTC.DIRSET = LED_RED | LED_GREEN | LED_BLUE;  // LED Ports als Ausgang
	
	//PORTC.OUT |= LED_GREEN; // LEDS an
	PORTC.OUT |= LED_GREEN; // LEDS an
	

	sync_osc();
	start_mcp_clock();
	spi_init();
	can_init();
	read_can_addr();
	
    InitializeUSARTC1();
	RTC_Init();
	
	Interrupt_Init();
	
	sendUSARTC1_putstr("DEBUG Interface\n\r");
	
	
	uint32_t x;
	uint16_t i;
	
	char buf[9];
	
	powermeter_SetSampleratePerPeriod(4);
	powermeter_Start();
	
	void * up;
	void * ip;

	while (1) {
		can_handler();
		
		{	//check if calculations has to be done
			if(powermeter.startCalculations == 2)
			{
				powermeter.startCalculations = 0;

				up = (void*)&powermeter.samplebuffer[powermeter.samplebuffer_page].u[0];
				ip =  (void*)&powermeter.samplebuffer[powermeter.samplebuffer_page].i1[0];
				
				/*for(i = 0;i < powermeter.ADCSampleBufferSize - (3 * sizeof(uint16_t));i+=(3*sizeof(uint16_t)))
				{*/
	
				/*	powermeter.powerdraw.c1.Ueff += *((uint16_t*)(up + i));
					powermeter.powerdraw.c2.Ueff += *((uint16_t*)(up + i + sizeof(uint16_t)));
					powermeter.powerdraw.c3.Ueff += *((uint16_t*)(up + i + sizeof(uint16_t)*2));

					powermeter.powerdraw.c1.Ieff += *((uint16_t*)(ip + i));
					powermeter.powerdraw.c2.Ieff += *((uint16_t*)(ip + i + sizeof(uint16_t)));
					powermeter.powerdraw.c3.Ieff += *((uint16_t*)(ip + i + sizeof(uint16_t)*2));	*/
/*
					itoa(*((uint16_t*)(up + i)),buf[0],10);
					sendUSARTC1_putstr(buf[0]);
					sendUSARTC1_putstr(" \n");
					itoa( *((uint16_t*)(up + i + sizeof(uint16_t))),buf[0],10);
					sendUSARTC1_putstr(buf[0]);
					sendUSARTC1_putstr(" \n");
					itoa(*((uint16_t*)(up + i + sizeof(uint16_t)*2)),buf[0],10);
					sendUSARTC1_putstr(buf[0]);
					sendUSARTC1_putstr(" \n");
	
					itoa(*((uint16_t*)(ip + i )),buf[0],10);
					sendUSARTC1_putstr(buf[0]);
					sendUSARTC1_putstr(" \n");
					itoa(*((uint16_t*)(ip + i + sizeof(uint16_t))),buf[0],10);
					sendUSARTC1_putstr(buf[0]);
					sendUSARTC1_putstr(" \n");
					itoa(*((uint16_t*)(ip + i + sizeof(uint16_t)*2)),buf[0],10);
					sendUSARTC1_putstr(buf[0]);
					sendUSARTC1_putstr(" \n");
				}*/
				
			//flip page
			if(powermeter.samplebuffer_page)
				powermeter.samplebuffer_page=0;
			else
				powermeter.samplebuffer_page=1;
			}
		}


		PORTC.OUTSET = LED_GREEN;	//set green LED, disable once a second (keep alive)
		PORTC.OUTCLR = LED_RED;	//clear RED LED, enable on transfer
		/*x++;
		if(x == 1000000){
			x = 0;
			
		}*/
	}
}

