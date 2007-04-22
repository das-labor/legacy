/**
 * Real-Time-Clock generic stuff
 * 
 * 
 * 
 * 
 * 
 * 
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

typedef struct{
	uint8_t	seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t dayofmonth;
	uint8_t month;
	int16_t year;
} timestamp;

#endif /*RTC_H_*/
