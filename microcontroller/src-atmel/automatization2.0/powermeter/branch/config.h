
#ifndef _H_CONFIG
#define _H_CONFIG
#define XMEGA
#define AVR


// can.[ch] defines
#define CAN_INTERRUPT

#define CAN_TX_BUFFER_SIZE 16
#define CAN_RX_BUFFER_SIZE 16

//CAN hacks
#define sendwithDELAY 0
#define laborhack 0

//ADCSamplesPerPeriod mode
#define USE_STATIC_ADCSamplesPerPeriod 1
#define ADCSAMPLESPERPERIOD 128

//debugmode
#define DEBUGMODE 0		//set to 1 for DEBUGMODE, otherwise 0

//offset compensation for ADC
#define ADC_OFFSET_CAL 1

//powermeter samplebuffer size (allows up to POWERMETER_SAMPLEBUFF samples per period)
#define POWERMETER_SAMPLEBUFF 300		//this will use POWERMETER_SAMPLEBUFF*36 bytes of RAM

#define NET_FREQ 50		//set freq of Powerline

#define MCP_CS_PORT   PORTD
#define MCP_CS_BIT        4

#define F_MCP ((F_CPU) / 2)		//use 16000000 or 8000000

//RGB LED on PORTC
#define LED_RED _BV(PIN2)
#define LED_GREEN _BV(PIN1)
#define LED_BLUE _BV(PIN0)

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_PIN_MCP_INT          1
#define SPI_REG_PIN_MCP_INT  PORTD

//for interrupt driven version
//active pull up
//trigger on falling edge
#define ENABLE_CAN_INT() PORTD.PIN1CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_LEVEL_gc; PORTD.INTCTRL = (PORTD.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) | PORT_INT1LVL_OFF_gc | PORT_INT0LVL_MED_gc; PORTD.INT0MASK=(1<<1);

#define MCP_INT_VEC PORTD_INT0_vect

 
#define XMEGA_SPI      SPID
#define AVR_SPI_PORT  PORTD
#define AVR_SS_BIT        4
#define AVR_MOSI_BIT      5
#define AVR_MISO_BIT      6
#define AVR_SCK_BIT       7


#define SPI_PIN_MOSI 5
#define SPI_PIN_MISO 6
#define SPI_PIN_SCK 7
#define SPI_PIN_SS 4

#if POWERMETER_SAMPLEBUFF * 36 > (15500 - (CAN_TX_BUFFER_SIZE + CAN_RX_BUFFER_SIZE) * 13)
	#error to less RAM, decrease POWERMETER_SAMPLEBUFF or CAN_TX_BUFFER_SIZE or CAN_RX_BUFFER_SIZE
#endif

#endif
