
// Ports
#define PORT_CMD		PORTC		// Command Output Register
#define DDR_CMD			DDRC		// Data Direction Register for Command Port

#define PIN_DATA		PINA		// Data Input Register
#define PORT_DATA		PORTA		// Data Output Register
#define DDR_DATA		DDRA		// Data Direction Register for Data Port

// Command Port Bits
#define D_I			0x04		// D/I Bit Number
#define R_W			0x03		// R/W Bit Number
#define EN			0x05		// EN Bit Number
#define CSEL0			0x00		// CS0 Bit Number
#define CSEL1			0x01		// CS1 Bit Number
