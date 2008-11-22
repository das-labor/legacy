/**
 * \file	persistentCounter.h
 * \author	Daniel Otte 
 * \brief	a persistent 24-bit vounter in EEPROM for ATmega µC
 * 
 * 
 */

#ifndef PERSISTENTCOUNTER_H_
#define PERSISTENTCOUNTER_H_


#include <stdint.h>

#define PERSISTENT_COUNTER_BITS 24

void percnt_init(void);
uint32_t percnt_get(void);
void percnt_inc(void);



#endif /*PERSISTENTCOUNTER_H_*/
