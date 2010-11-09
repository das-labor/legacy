#include <stdio.h>
#include <expat.h>
#include <stdlib.h>
#include <strings.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "vector.h"

void map_load (const char*);
void pointdb_drawpoints ();
void map_set_pos_delta (double x, double y, double z);
void map_draw();
