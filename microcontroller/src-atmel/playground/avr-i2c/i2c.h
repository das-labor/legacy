#ifndef I2C_H_
#define I2C_H_

#include "config.h"


//#include <util/twi.h>

#define TWI_WAIT_READY() while (! (_BV(TWINT) & TWCR))
/* keep acknowlege state! */
#define TWI_GO_ON(a)		TWCR = _BV(TWINT) | _BV(TWEN)  /*| (TWCR & _BV(TWEA)) | (a)?(_BV(TWEA)):0 */
#define TWI_START(a)	 	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) /*| (TWCR & _BV(TWEA)) | (a)?(_BV(TWEA)):0 */
#define TWI_STOP()	 	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN) 
#define TWI_SEND(s)	 	TWDR=(s)
#define TWI_GET(s)	 	(s)=TWDR
#define TWI_STATUS	 	(TWSR&(~(_BV(2)|_BV(1)|_BV(0))))

#define I2C_READ 1
#define I2C_WRITE 0

#define HIGH(s) ((s)>>8)
#define LOW(s)  ((s)&0xFF)

#define I2C_SPEED_100			72,0
#define I2C_SPEED_SLOWEST		255,3
#define I2C_SPEED_FASTEST		10,0		


typedef uint8_t i2c_addr_t;
typedef uint8_t i2c_status_t;
typedef enum {I2C_MT, I2C_MR, I2C_ST, I2C_SR, I2C_STOPPED} i2c_mode_t;

#ifndef __BOOL__
#ifndef BOOL
	typedef enum{false=0, true=1} bool;
	#define BOOL
	#define __BOOL__
#endif
#endif

void			i2c_init();
void			i2c_set_speed(uint8_t, uint8_t);
i2c_status_t	i2c_start (i2c_addr_t);
i2c_status_t	i2c_stop();
i2c_status_t	i2c_sendbyte(uint8_t byte);
uint8_t			i2c_readbyte();

/*
 * Nice functions
 */
i2c_status_t i2c_restart (i2c_addr_t);
i2c_status_t i2c_start_a (i2c_addr_t devaddr);
i2c_status_t i2c_restart_a(i2c_addr_t devaddr);

void i2c_do_ack();
void i2c_do_not_ack();

#endif /*I2C_H_*/
