#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "vector.h"

double vec3len (vec3_t in_v)
{
	return sqrt (in_v[0] * in_v[0] + in_v[1] * in_v[1] + in_v[2] * in_v[2]);
}

void vec3normalize (vec3_t in_v)
{
	double len;

	len = vec3len (in_v);
	in_v[0] /= len;
	in_v[1] /= len;
	in_v[2] /= len;
}

void vec3add (vec3_t out_v, vec3_t in_v0, vec3_t in_v1)
{
	out_v[0] = in_v0[0] + in_v1[0];
	out_v[1] = in_v0[1] + in_v1[1];
	out_v[2] = in_v0[2] + in_v1[2];
}

void vec3scale (vec3_t in_v, double in_scale)
{
	in_v[0] *= in_scale;
	in_v[1] *= in_scale;
	in_v[2] *= in_scale;
}

void vec3xyortho (vec3_t out_result, vec3_t in_v0, vec3_t in_v1)
{
	/* swap & diff */
	out_result[0] = (in_v0[1]-in_v1[1]);
	out_result[1] = (in_v0[0]-in_v1[0]);
	out_result[2] = 0.0f;
	out_result[0] *= -1;
	vec3normalize (out_result);
}

void vec3inv (vec3_t in_v)
{
	in_v[0] *= -1;
	in_v[1] *= -1;
	//in_v[2] *= -1;
}

void vec3sub (vec3_t out_res, vec3_t in_v0, vec3_t in_v1)
{
	out_res[0] = in_v0[0] - in_v1[0];
	out_res[1] = in_v0[1] - in_v1[1];
	out_res[2] = in_v0[2] - in_v1[2];
}

/* search nearest pair of vectors */
void vec3getnearestpoint (vec3_t out_res, vec3_t in_ref,
	uint_fast32_t num_vectors, vec3_t *in_v)
{
	double min_len = 1024.0f, tmplen;
	uint_fast32_t i, min_idx = 0;
	vec3_t tmp;
	
	for (i=0;i<num_vectors;i++)
	{
		vec3sub (tmp, in_v[i], in_ref);
		tmplen = vec3len (tmp);
		if (tmplen < min_len)
		{
			min_len = tmplen;
			min_idx = i;
		}
	}
	memcpy (out_res, in_v[min_idx], sizeof(vec3_t));
}

double vec3angle2d (vec3_t in_v0, vec3_t in_v1)
{
	vec3_t a,b;
	double dp; /* dot product */

	memcpy (a, in_v0, sizeof(vec3_t));
	memcpy (b, in_v1, sizeof(vec3_t));

	b[2] = 0.0f;
	a[2] = 0.0f;

	vec3normalize(a);
	vec3normalize(b);
	
	dp = a[0]*b[0] + a[1]*b[1];
	return acos (dp);
}

void vec3sort4clockwise2d (vec3_t *out_result, vec3_t *in_v)
{
	uint_fast8_t exidx = 0, i, donemsk = 0x00;
	
	double extremum = -100000.0f;
	vec3_t tmp[4];
	
	/* north */
	for (i=0;i<4;i++)
	{
		if (in_v[i][1] > extremum)
		{
			exidx = i;
			extremum = in_v[i][1];
		}
	}

	donemsk |= (0x1<<exidx);
	memcpy (tmp[0], in_v[exidx], sizeof(vec3_t));
	extremum = -1000000.0f;

	/* east */
	for (i=0;i<4;i++)
	{
		if (in_v[i][0] > extremum && !((0x1 << i) & donemsk))
		{
			exidx = i;
			extremum = in_v[i][0];
		}
	}
	
	donemsk |= (0x1<<exidx);
	memcpy (tmp[1], in_v[exidx], sizeof(vec3_t));
	extremum = 1000000.0f;
	
	/* south */
	for (i=0;i<4;i++)
	{
		if (in_v[i][1] < extremum && !((0x1 << i) & donemsk))
		{
			exidx = i;
			extremum = in_v[i][1];
		}
	}
	
	donemsk |= (0x1<<exidx);
	memcpy (tmp[2], in_v[exidx], sizeof(vec3_t));
	
	i=0;
	while (donemsk & (0x1<<i))
	{
		i++;
	}

	memcpy (tmp[3], in_v[i], sizeof(vec3_t));
	memcpy (out_result, tmp, sizeof(vec3_t)*4);
}
