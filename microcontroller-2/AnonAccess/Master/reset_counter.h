#ifndef RESET_COUNTER_H_
#define RESET_COUNTER_H_

#include <stdint.h>

uint64_t resetcnt_read(void);
void resetcnt_inc(void);


#endif /*RESET_COUNTER_H_*/
