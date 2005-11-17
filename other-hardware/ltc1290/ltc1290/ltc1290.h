#ifndef LTC1290_H
#define LTC1290_H

#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#define LTCSLEEP 100

/**
 * - Differential == 0
 *      Channel directly addresses a input channel 
 *
 * - Differential == 1
 *      Channel      0 1 2 3 4 5 6 7
 *         0         + -
 *         1         - +
 *         2             + -
 *         3             - +
 *         4                 + -
 *         5                 - +
 *         6                     + -
 *         7                     - +
 *
 */
typedef struct ltc1290_plan {
	int      single;
	int      channel;
	int      unipolar;
	uint16_t value;
	double   dvalue;
} ltc1290_plan_t;


int  ltc1290_open(char *port);
void ltc1290_close();
void ltc1290_csoff();
int  ltc1290_clk(int din);
void ltc1290_measure(ltc1290_plan_t plan[], int count);

#endif
