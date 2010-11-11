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

typedef struct flight_s
{
	fpoint_t dir;
	float pitch, roll, yaw;
	float speed;
	vec3_t left;
} flight_t;

/* move further towards the next point on the route in percent.
 */
void flightroute_iterate_percent (froute_t *io_flight_p, float in_percent);

void flightroute_set_view (froute_t *in_point);
void flight_set_view (flight_t *in_f);
void flight_set_xydelta (flight_t *in_f, int in_x, int in_y);
void flight_iterate_percent (flight_t *in_f, float in_percent);
void flight_init (flight_t *in_f);

void flight_set_pitch (flight_t *fp, int);
void flight_set_roll (flight_t *fp, int);

/* calc transition vector
 */
void flight_gen_transv (froute_t *in_route);

/* generate a flight route around a point
 */
froute_t* froute_around_point (vec3_t in_p, int num_points);
