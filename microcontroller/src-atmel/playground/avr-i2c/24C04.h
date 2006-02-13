#ifndef 24C04_H_
#define 24C04_H_


#include <stdint.h>
#include "i2c.h"

typedef uint8_t E24C04_addr_t;


void 	E24C04_init();
void 	E24C04_byte_write (i2c_addr_t devaddr, E24C04_addr_t addr, uint8_t data);
void 	E24C04_page_write (i2c_addr_t devaddr, E24C04_addr_t addr, uint8_t *data, uint16_t length);
uint8_t E24C04_cur_byte_read(i2c_addr_t devaddr);
void 	E24C04_cur_block_read(i2c_addr_t devaddr, E24C04_addr_t addr, uint8_t *buffer, uint16_t length);
uint8_t E24C04_byte_read(i2c_addr_t devaddr, E24C04_addr_t addr);
void 	E24C04_block_read(i2c_addr_t devaddr, E24C04_addr_t addr, uint8_t *buffer, uint16_t length);


#endif /*24C04_H_*/
