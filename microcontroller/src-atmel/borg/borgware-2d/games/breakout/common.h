#ifndef COMMON_H
#define COMMON_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../joystick/joystick.h"
#include "../../config.h"
#include "../../autoconf.h"
#include "../../compat/eeprom.h"
#include "../../random/prng.h"
#include "../../compat/pgmspace.h"
#include "../../util.h"
#include "../../menu/menu.h"
#include "../../pixel.h"
#include "config.h"
#include "playfield.h"
#include "ball.h"
#include "score.h"
#include "level.h"
#include "rebound.h"
#include "messages.h"

#define MAX(a,b) (a > b) ? a : b
#endif /* COMMON_H */
