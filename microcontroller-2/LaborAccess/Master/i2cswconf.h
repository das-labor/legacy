
// clock line port
#define SCLPORT	PORTC	// i2c clock port
#define SCLDDR	DDRC	// i2c clock port direction
// data line port
#define SDAPORT	PORTC	// i2c data port
#define SDADDR	DDRC	// i2c data port direction
#define SDAPIN	PINC	// i2c data port input
// pin assignments
#define SCL		PC0		// i2c clock pin
#define SDA		PC1		// i2c data pin

//i2c address of eeprom
#define EEPROM_ADDRESS 0xa0

//set this for EEPROMS, that need 2 address bytes (64k)
#define EEPROM_2_ADDR_BYTES

//use typical values depending on EEPROM type
#ifdef EEPROM_2_ADDR_BYTES
	#define EEPROM_SECTOR_SIZE 128
#else
	#define EEPROM_SECTOR_SIZE 4
#endif

//set this to get the detection funtion for I2C Cards
//#define EE_EDETCT
