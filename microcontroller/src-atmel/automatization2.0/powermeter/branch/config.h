

#define XMEGA
#define AVR


// can.[ch] defines
#define CAN_INTERRUPT
#define CAN_TX_BUFFER_SIZE 64
#define CAN_RX_BUFFER_SIZE 64


#define MCP_CS_PORT   PORTD
#define MCP_CS_BIT        4

#define F_MCP ((F_CPU) / 2)

#define LED_BLUE _BV(PIN2)		//Pin2 = blau
#define LED_RED _BV(PIN1)		//pin1 = rot
#define LED_GREEN _BV(PIN0)		//pin0 = gruen

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_PIN_MCP_INT          1
#define SPI_REG_PIN_MCP_INT  PORTD

//for interrupt driven version
//active pull up
//trigger on falling edge
//#define ENABLE_CAN_INT() PORTD.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_LEVEL_gc  ; PORTD.INTCTRL = (PORTD.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) | PORT_INT1LVL_OFF_gc | PORT_INT0LVL_MED_gc; PORTD.INT0MASK=(1<<1);

#define ENABLE_CAN_INT() PORTD.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc ; PORTD.INTCTRL = (PORTD.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) | PORT_INT1LVL_OFF_gc | PORT_INT0LVL_MED_gc; PORTD.INT0MASK=(1<<1);
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