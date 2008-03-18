
// clock line port
#define SCLPORT	PORTA	// i2c clock port
#define SCLDDR	DDRA	// i2c clock port direction
// data line port
#define SDAPORT	PORTA	// i2c data port
#define SDADDR	DDRA	// i2c data port direction
#define SDAPIN	PINA	// i2c data port input
// pin assignments
#define SCL		PA6		// i2c clock pin
#define SDA		PA7		// i2c data pin

//i2c address of eeprom
#define EEPROM_ADDRESS 0xa0

#define EEPROM_SECTOR_SIZE 8

#define EE_DETECT
