#ifndef UTIL_H
#define UTIL_H

#include "config.h"

/**
 * #define F_CPU 16000000
 */


#ifndef F_CPU
  #define F_CPU 16000000
#endif

void wait(int ms);

#endif
