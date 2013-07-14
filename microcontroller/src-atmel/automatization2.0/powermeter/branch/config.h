
#ifndef _H_CONFIG
#define _H_CONFIG
#define XMEGA
#define AVR


// can.[ch] defines
#define CAN_INTERRUPT

// amount of packet buffers
#define CAN_TX_BUFFER_SIZE 16
#define CAN_RX_BUFFER_SIZE 16

//CAN hacks
#define sendwithDELAY 0
#define laborhack 0

//ADCSamplesPerPeriod mode
//this value must be smaller than 256 ! since DMA controller only supports 255 transfers at once
#define ADCSAMPLESPERPERIOD 128
#define ADCSAMPLESSHIFT 7

//debugmode
#define DEBUGMODE 0		//set to 1 for DEBUGMODE, otherwise 0

//offset compensation for ADC
#define ADC_OFFSET_CAL 1

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
#define ENABLE_CAN_INT() PORTD.INT0MASK = (1 << 1);

#define DISABLE_CAN_INT() PORTD.INT0MASK = 0;

#define MCP_INT_VEC PORTD_INT0_vect
#define SETUP_CAN_INT() PORTD.PIN1CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_LEVEL_gc; PORTD.INTCTRL = (PORTD.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) | PORT_INT1LVL_OFF_gc | PORT_INT0LVL_MED_gc;

#define EEPROM_LAP_ADDR 0x00

#define XMEGA_SPI      SPID
#define AVR_SPI_PORT  PORTD
#define AVR_SS_BIT        4
#define AVR_MOSI_BIT      5
#define AVR_MISO_BIT      6
#define AVR_SCK_BIT       7

#if ADCSAMPLESPERPERIOD * 36 > (15500 - (CAN_TX_BUFFER_SIZE + CAN_RX_BUFFER_SIZE) * 13)
	#error to less RAM, decrease POWERMETER_SAMPLEBUFF or CAN_TX_BUFFER_SIZE or CAN_RX_BUFFER_SIZE
#endif

#endif
