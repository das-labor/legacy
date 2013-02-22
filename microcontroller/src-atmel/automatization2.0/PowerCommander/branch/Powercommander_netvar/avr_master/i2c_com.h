#ifndef _I2C_COM_H
#define _I2C_COM_H

extern void sync_output_cache(void);
extern void update_slave_outputs(void);
extern void set_output(uint8_t num, uint8_t val);
extern void set_pwm(uint8_t num, uint8_t val);

#endif // _I2C_COM_H

