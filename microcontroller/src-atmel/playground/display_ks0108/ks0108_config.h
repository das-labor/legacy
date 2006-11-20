
/*Pinout von HQM1286404 Display (Dario):
1		Vcc
2		GND
3		Vee (mittelabgriff von 20k Poti) - es wird anscheinend kein Kondensator gebraucht.
4-11	DB0-DB7
12		CS0
13		CS1
14		RESET
15		R/W		READ=1
16		D/I
17		EN
18		Vee out - über nen 20k Poti nach Masse auf Pin3
19		LED Anode		80mA/4,0V
20		LED Kathode
*/


// Ports
#define PORT_CMD		PORTC		// Command Output Register
#define DDR_CMD			DDRC		// Data Direction Register for Command Port

#define PIN_DATA		PINA		// Data Input Register
#define PORT_DATA		PORTA		// Data Output Register
#define DDR_DATA		DDRA		// Data Direction Register for Data Port

// Command Port Bits
#define CSEL0				0x00		// CS0 Bit Number
#define CSEL1				0x01		// CS1 Bit Number
#define R_W					0x03		// R/W Bit Number
#define D_I					0x04		// D/I Bit Number
#define EN					0x05		// EN Bit Number
