/*############################################################################
############################################################################*/

#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

//############################################################################

// I2C Konstanten
#define SCL_CLOCK  200000L
#define I2C_TIMEOUT 30000
#define I2C_START          0x08
#define I2C_REPEATED_START 0x10
#define I2C_TX_SLA_ACK     0x18
#define I2C_TX_DATA_ACK    0x28
#define I2C_RX_SLA_ACK     0x40
#define I2C_RX_DATA_ACK    0x50

//############################################################################

extern unsigned char twi_state;
extern unsigned char motor;
extern unsigned char motorread;
extern unsigned char motor_rx[8];

void i2c_reset(void);
extern void i2c_init (void); // I2C initialisieren
extern char i2c_start (void); // Start I2C
extern void i2c_stop (void); // Stop I2C
extern char i2c_write_byte (char byte); // 1 Byte schreiben
extern void i2c_reset(void);

#endif
