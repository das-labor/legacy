#ifndef POWERCOMMANDER_FKT
#define POWERCOMMANDER_FKT

#include "PowerCommander.h"

void switch_fkt(struct t_i2cproto* i2cproto);
void pwm_fkt(struct t_i2cproto* i2cproto);
void switch_on(volatile uint8_t *port,uint8_t pin);
void switch_off(volatile uint8_t *port,uint8_t pin);
void switch_status(volatile uint8_t *port,uint8_t pin, uint8_t *result);
void pwm_set(volatile uint8_t *port,uint8_t value);
void pwm_get(volatile uint8_t *port,uint8_t *result);
void virt_fkt(struct t_i2cproto* i2cproto);
void virt_power(struct t_i2cproto* i2cproto);
void virt_power_on();
void virt_power_off();
void virt_power_status(struct t_i2cproto* i2cproto);
void virt_vortrag(struct t_i2cproto* i2cproto);
void virt_vortrag_on();
void virt_vortrag_off();
void virt_vortrag_pwm_set(struct t_i2cproto* i2cproto);
#endif

