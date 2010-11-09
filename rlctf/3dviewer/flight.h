#include <stdint.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <math.h>

#pragma once

#include "vector.h"

typedef struct fpoint_s
{
	vec3_t eye;
	vec3_t target;
	vec3_t up;
} fpoint_t;

typedef struct froute_s
{
	float progress;
	fpoint_t transv;
	fpoint_t *p;
	struct froute_s *next;

} froute_t;

/* move further towards the next point on the route in percent.
 */
void flight_iterate_percent (froute_t *io_flight_p, float in_percent);

void flight_set_view (froute_t *in_point);

/* calc transition vector
 */
void flight_gen_transv (froute_t *in_route);
