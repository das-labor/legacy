/* locodoco packet handling functions
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#pragma once
static struct ldc_state
{
	uint8_t txlen;
}

void ldc_packet_handler ();
