#include "../common/rfm12usb_config.h"
#include <stdint.h>
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>
#include "rfm12.h"
#include "../common/requests.h"

#ifndef RFMUSB_OOK_MAXLEN
	#define RFMUSB_OOK_MAXLEN 32
#endif

void ook_send (uint8_t in_bits, uint16_t in_delay, uint8_t *in_data, uint8_t in_type);
void ook_queue_msg (uint8_t *in_msg);
void ook_tick ();
