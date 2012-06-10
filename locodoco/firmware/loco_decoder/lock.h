/* resource locking funktions
 */

#include <stdint.h>

#pragma once

typedef uint8_t lock_t;

#define LOCK_ADC 0x00
#define LOCK_ADC_MASK 0x01

uint8_t try_lock_get (lock_t in_type);
void lock_release (lock_t in_type);
