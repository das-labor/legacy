#include <avr/io.h>
#include "rfm12_hw.h"
.include "rfm12_config.inc"


; common definitions for internal use of the rfm12 library.
; you usually don't need to change anything in here. in case
; you're looking for the configuration, see "rfm12_config.inc"

REG_SPI_DATA=r10

