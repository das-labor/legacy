
// clock line port
#define SCLPORT	PORTC	// i2c clock port
#define SCLDDR	DDRC	// i2c clock port direction
// data line port
#define SDAPORT	PORTD	// i2c data port
#define SDADDR	DDRD	// i2c data port direction
#define SDAPIN	PIND	// i2c data port input
// pin assignments
#define SCL		PC2		// i2c clock pin
#define SDA		PD2		// i2c data pin

//i2c address of eeprom
#define EEPROM_ADDRESS 0xa0
