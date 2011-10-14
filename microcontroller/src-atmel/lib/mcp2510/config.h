/*
 * Basic config header to suppress compiler errors.
 */

#define F_CPU 16000000

// Pin PB2 muss Ausgang sein wenn nicht als SS verwendet
#define MCP_CS        PB4
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
//#define CAN_INTERRUPT


#define F_MCP F_CPU
