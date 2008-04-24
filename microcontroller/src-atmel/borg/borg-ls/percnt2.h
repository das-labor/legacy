#ifndef PERCNT2_H_
#define PERCNT2_H_

#include <stdint.h>
#include "config.h"


#define PERCNT_OK 0
#define PERCNT_ERROR 1
#define PERCNT_HAVYERROR 2
#define PERCNT_INITERROR 23
#define PERCNT_STRANGEERROR 42

void percnt_reset(uint8_t counter);
uint32_t percnt_get(uint8_t counter);
uint8_t percnt_inc(uint8_t counter);
uint8_t percnt_init(uint8_t counter);

#endif /*PERCNT2_H_*/
