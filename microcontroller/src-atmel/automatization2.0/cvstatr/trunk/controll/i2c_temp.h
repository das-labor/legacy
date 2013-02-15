#ifndef _I2C_TEMP_H
#define _I2C_TEMP_H


extern void init_sensor(uint8_t addr);
extern void get_temp(uint8_t *p, uint8_t addr);

/* Initiates temperature conversions. If the part is in one-shot mode (1SHOT = 1), only one conversion is
 * performed. In continuous mode (1SHOT = 0), continuous temperature conversions are performed until a
 * Stop Convert T command is issued.
 */
#define START_CONVERT		0x51

// Stops temperature conversions when the device is in continuous conversion mode (1SHOT = 0).
#define STOP_CONVERT		0x22

// Reads last converted temperature value from the 2-byte temperature register.
#define READ_TEMPERATURE	0xaa

// Reads or writes the 2-byte TH register.
#define ACCESS_TH		0xa1

// Reads or writes the 2-byte TL register.
#define ACCESS_TL		0xa2

// Reads or writes the 1-byte configuration register.
#define ACCESS_CONFIG		0xac

/* Initiates a software power-on-reset (POR), which stops temperature conversions and resets all registers
 * and logic to their power-up states. The software POR allows the user to simulate cycling the power
 * without actually powering down the device.
 */
#define SOFTWARE_POR		0x54

#endif // _I2C_TEMP

