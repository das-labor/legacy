#ifndef __E24CXXX_H_
#define __E24CXXX_H_

#include <stdint.h>
#include "i2c.h"

typedef uint16_t E24C_addr_t;


void 	E24C_init(void);
void 	E24C_byte_write (i2c_addr_t devaddr, E24C_addr_t addr, uint8_t data);
void 	E24C_page_write (i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *data, uint16_t length);
uint8_t E24C_cur_byte_read(i2c_addr_t devaddr);
void 	E24C_cur_block_read(i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *buffer, uint16_t length);
uint8_t E24C_byte_read(i2c_addr_t devaddr, E24C_addr_t addr);
void 	E24C_block_read(i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *buffer, uint16_t length);

#endif /*__E24CXXX_H_*/

