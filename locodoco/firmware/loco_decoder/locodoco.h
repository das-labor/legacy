/* locodoco packet handling functions
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "../../common/protocol.h"
#include "rfm12.h"
#pragma once



void ldc_packet_handler (void);
