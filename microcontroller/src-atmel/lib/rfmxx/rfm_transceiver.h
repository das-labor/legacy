#include "rfm_config.h"

#include <avr/io.h>

#define RFM_MODE_TX  0x00
#define RFM_MODE_RX  0x01
#define RFM_MODE_OFF 0x03

void rfm_setMode (uint8_t in_mode);
void rfm_init(void);
