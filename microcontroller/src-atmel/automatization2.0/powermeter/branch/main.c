#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "can_handler.h"
#include "can/spi.h"
#include "can/can.h"
#include "adc_driver.h"
#include "led_driver.h"
#include "config.h"
#include "powermeter_driver.h"
#include "ursartC1_driver.h"
#include "event_system_driver.h"
#include "error_handler.h"
#include "rtc_driver.h"
#include "wdt_driver.h"

void sync_osc()
{
	/*32 MHz Oszillator starten */
	OSC.CTRL |= OSC_RC32MEN_bm;
	
	/*Wenn Oscillator stabil wird das Flag RC32MRDY
	 * gesetzt und 32Mhz können benutzt werden*/
	while (!(OSC.STATUS & OSC_RC32MRDY_bm));

	/* auto kalibrierung ein */
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	/*Clock auf 32Mhz einstellen*/
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

void start_mcp_clock()
{
	//init the timer for mcp2515 clock
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

void Eventsystem_init(void)
{
	/* Select multiplexer input. */
	EVSYS_SetEventSource( 0, EVSYS_CHMUX_OFF_gc);
	EVSYS_SetEventSource( 1, EVSYS_CHMUX_OFF_gc);
	EVSYS_SetEventSource( 2, EVSYS_CHMUX_OFF_gc);
	EVSYS_SetEventSource( 3, EVSYS_CHMUX_OFF_gc);
	EVSYS_SetEventSource( 4, EVSYS_CHMUX_OFF_gc);
	EVSYS_SetEventSource( 5, EVSYS_CHMUX_OFF_gc);
	EVSYS_SetEventSource( 6, EVSYS_CHMUX_OFF_gc);
	EVSYS_SetEventSource( 7, EVSYS_CHMUX_TCC1_OVF_gc);	//event Timer1CC1_OVF
}

void Interrupt_Init(void)
{
	//enable ROUND ROBIN,enable MED_LVL & LOW_LVL interrupts !!!!
	uint8_t tmp = PMIC_RREN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = tmp;

	sei();	//global allow interrupts
}

int main(void)
{
	LED_initPORTC();  // LED Ports als Ausgang
	LED_isrunning();	//set green LED

	sync_osc();
	start_mcp_clock();
	spi_init();
	can_init();
	read_can_addr();

	Eventsystem_init();
	Interrupt_Init();

	setERROR(0);
	can_send_packet = 0;

	//init watchdog
	WDT_EnableAndSetTimeout(WDT_PER_64CLK_gc);

	powermeter_SetSampleratePerPeriod(ADCSAMPLESPERPERIOD);	//configure ADCSAMPLESPERPERIOD in config.h
	powermeter_Start();

	uint16_t x = 0;
	while (1) {
		can_handler();
		powermeter_docalculations();
		if (can_send_packet)
		{
			can_createDATAPACKET();
			can_send_packet = 0;
		}
		WDT_Reset();
		if ((RTC.CNT & 0x00ff) >= x)
			x = RTC.CNT;
		else
		{		//this is executed 4 times per second
			ERROR_blink();
		}
	}
}

