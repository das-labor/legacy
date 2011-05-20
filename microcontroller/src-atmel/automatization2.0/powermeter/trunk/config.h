

#define XMEGA
#define AVR

#define MCP_CS_PORT   PORTD
#define MCP_CS_BIT        4

// can.[ch] defines
#define F_MCP ((F_CPU) / 2)

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_PIN_MCP_INT          1
#define SPI_REG_PIN_MCP_INT  PORTD
 
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