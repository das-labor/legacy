#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "can_handler.h"
#include "can/spi.h"
#include "can/can.h"
//#include "adc_driver.h"
#include "rtc_driver.h"
//#include "tc_driver.h"
//#include "dma_driver.h"
#include "ursartC1_driver.h"
#include "led_driver.h"
#include "tests.h"
#include "event_system_driver.h"
#include "error_handler.h"


void sync_osc() {
	
	/*32 MHz Oszillator starten */
	OSC.CTRL |= OSC_RC32MEN_bm;
	
	/*Wenn Oscillator stabil wird das Flag RC32MRDY
	 * gesetzt und 32Mhz können benutzt werden*/
	while (!(OSC.STATUS & OSC_RC32MRDY_bm));

	/* auto kalibierung ein */
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

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
#if F_MCP == 16000000
	TCD0.CTRLA = 1; //clk/2
#elif F_MCP == 8000000
	TCD0.CTRLA = 2; //clk/2
#endif
	
}


#define PORT_GRAPH 0x36
/*
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

}*/

void Interrupt_Init(void)
{
	//enable ROUND ROBIN,enable MED_LVL & LOW_LVL interrupts !!!!
	uint8_t tmp= PMIC_RREN_bm|PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = tmp; 

	sei();	//global allow interrupts
}

int main(void)
{
	LED_initPORTC();  // LED Ports als Ausgang
	LED_isrunning();	//set green LED
	
	sync_osc();		//start 32Mhz clock
#if DEBUGMODE
    InitializeUSARTC1();	//init USARTC1 (for debuging)	
	sendUSARTC1_putstr("DEBUG Interface\n\r");
#endif
	start_mcp_clock();	//start PWM, 16Mhz/8Mhz CLK for MCP
	spi_init();		//init SPI
	can_init();		//init MCP (CAN)
	read_can_addr();	//init CAN
	//RTC_Init();	//init Real-Time-Clock
	//Interrupt_Init();	//init the Interruptsystem
	//Eventsystem_init();	//setup eventsystem
	

	
	uint32_t x;
	while (1) {
		//spi_send(0xAA);
		can_handler();
		LED_on();		//turn status LED on
	}
	
	powermeter_SetSampleratePerPeriod(4);
	powermeter_Start();
	
	while (1) {
		can_handler();
		powermeter_docalculations();
		
		//if(getERROR())		//check for errors
		//	LED_error_on()	//turn red LED on
		//else
			LED_on();		//turn status LED on

		
	/*	
		x++;
		if(x == 200000){
			x = 0;
		}*/
	}
}

