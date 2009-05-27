
// clock line port
#define SCLPORT	PORTC	// i2c clock port
#define SCLDDR	DDRC	// i2c clock port direction
// data line port
#define SDAPORT	PORTC	// i2c data port
#define SDADDR	DDRC	// i2c data port direction
#define SDAPIN	PINC	// i2c data port input
// pin assignments
#define SCL		PC1		// i2c clock pin
#define SDA		PC0	// i2c data pin

//i2c address of eeprom
#define EEPROM_ADDRESS 0xa0

#define EEPROM_SECTOR_SIZE 8

#define EE_DETECT
