#ifndef POWERCOMMAND_OPTO_H
#define POWERCOMMAND_OPTO_H

#include "PowerCommander.h"

uint8_t opto00_off(struct t_status * data);
uint8_t opto00_on(struct t_status * data);
uint8_t opto00_status(struct t_status * data);
uint8_t opto01_off(struct t_status * data);
uint8_t opto01_on(struct t_status * data);
uint8_t opto01_status(struct t_status * data);

#endif
