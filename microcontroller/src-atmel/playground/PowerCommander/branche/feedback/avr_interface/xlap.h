/* -*- Mode: C; tab-width: 2 -*- */
#include "lap.h"

AVRX_EXTERNTASK(cancom_in);
AVRX_EXTERNTASK(cancom_out);
AVRX_EXTERNTASK(i2ccom_in);
AVRX_EXTERNTASK(i2ccom_out);

extern uint8_t myaddr;

void xlap_init();
