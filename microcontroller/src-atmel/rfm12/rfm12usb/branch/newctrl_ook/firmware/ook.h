#include "../common/rfm12usb_config.h"
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "rfm12.h"

#define OOK_RAW  0x00 /* send given bits as they are */
#define OOK_2722 0x01 /* encode 1 to 110, 0 to 100, add trailing bit, add trailing delay */

void send_ook (uint8_t in_bits, uint16_t in_delay, uint8_t *in_data, uint8_t in_type);
