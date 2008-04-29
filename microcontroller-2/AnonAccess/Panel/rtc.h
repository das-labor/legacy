#ifndef RTC_H_
#define RTC_H_
/*
 * a simple rtc implementation to make timestamps
 * 
 * 
 * 
 */
#include <stdint.h>
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

typedef uint64_t timestamp_t;

void rtc_init(void);
timestamp_t gettimestamp(void);
timestamp_t abstimedifference(timestamp_t t1, timestamp_t t2);

#endif /*RTC_H_*/
