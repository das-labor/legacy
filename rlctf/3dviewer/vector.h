#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#pragma once

typedef double vec3_t[3];
double vec3len (vec3_t in_v);
void vec3normalize (vec3_t in_v);
void vec3xyortho (vec3_t out_result, vec3_t in_v0, vec3_t in_v1);
void vec3scale (vec3_t in_v, double in_scale);
void vec3inv (vec3_t in_v);
void vec3add (vec3_t out_v, vec3_t in_v0, vec3_t in_v1);
void vec3getnearest (vec3_t out_res, vec3_t in_ref, vec3_t in_v0, vec3_t in_v1);
void vec3sub (vec3_t out_res, vec3_t in_v0, vec3_t in_v1);
double vec3angle2d (vec3_t in_v0, vec3_t in_v1);

/* sort vectors clockwise (north->east->south->west) on a 2d-plane
 */
void vec3sort4clockwise2d (vec3_t *out_result, vec3_t *in_v);
