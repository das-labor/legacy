#include <stdint.h>
#ifndef LIGHTS_H
#define LIGHTS_H
/* @description Initialize the pins/ports
 */
void lights_init();

/* @description Simply set lights according to bitmask
 */
void lights_set_raw (uint8_t in_bits);

#endif /* LIGHTS_H */
