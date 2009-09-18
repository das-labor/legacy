/* -*- Mode: C; tab-width: 2 -*- */
#ifndef POWERCOMMANDER_BRIGHT_H
#define POWERCOMMANDER_BRIGHT_H

#include "PowerCommander.h"

uint8_t bright_tafel_set(struct t_status *data);
uint8_t bright_beamer_set(struct t_status *data);
uint8_t bright_schraenke_set(struct t_status *data);
uint8_t bright_flipper_set(struct t_status *data);
uint8_t bright_free_set(struct t_status *data);
uint8_t bright_lounge_set(struct t_status *data);


#endif
