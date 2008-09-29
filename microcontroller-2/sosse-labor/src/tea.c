/************************************************

The Tiny Encryption Algorithm (TEA) by David Wheeler and Roger Needham
of the Cambridge Computer Laboratory

Placed in the Public Domain by David Wheeler and Roger Needham.

**** ANSI C VERSION ****

Notes:

TEA is a Feistel cipher with XOR and and addition as the non-linear
mixing functions.

Takes 64 bits of data in v[0] and v[1].  Returns 64 bits of data in w[0]
and w[1].  Takes 128 bits of key in k[0] - k[3].

TEA can be operated in any of the modes of DES. Cipher Block Chaining is,
for example, simple to implement.

n is the number of iterations. 32 is ample, 16 is sufficient, as few
as eight may be OK.  The algorithm achieves good dispersion after six
iterations. The iteration count can be made variable if required.

Note this is optimised for 32-bit CPUs with fast shift capabilities. It
can very easily be ported to assembly language on most CPUs.

delta is chosen to be the real part of (the golden ratio Sqrt(5/4) -
1/2 ~ 0.618034 multiplied by 2^32).

************************************************/

/*! @file
	\brief TEA functions.

	$Id: tea.c,v 1.6 2003/04/02 23:57:54 m Exp $
*/

#define TEA_SMALL

#include <config.h>
#include <tea.h>
#include <tools.h>
#ifdef DEBUG
#include <stdio.h>
#endif

#define hton_ul(x,y)
//#include <log.h>

#if CONF_WITH_CRYPT_ALGO==0

#if defined(TEA_SMALL)
uint32_t tea_func( uint32_t *in, uint32_t *sum, uint32_t *k )
{
	return ((*in<<4)+k[0]) ^ (*in+*sum) ^ ((*in>>5)+k[1]);
}
#endif

void tea_enc( uint32_t *v, uint32_t *k )
{
	uint32_t y, z;
	uint32_t sum=0;
	uint8_t n=ROUNDS;

	hton_ul(k,4);
	hton_ul(v,2);

#ifdef DEBUG
	printf( "tea_enc( %.8lX %.8lX, %.8lX %.8lX %.8lX %.8lX )\n", v[0], v[1],
		k[0], k[1], k[2], k[3] );
#endif

	y=v[0]; z=v[1];

	while(n-->0) {
		sum += DELTA;
#if defined(TEA_SMALL)
		y += tea_func( &z, &sum, &(k[0]) );
		z += tea_func( &y, &sum, &(k[2]) );
#else
		y += ((z<<4)+k[0]) ^ (z+sum) ^ ((z>>5)+k[1]);
		z += ((y<<4)+k[2]) ^ (y+sum) ^ ((y>>5)+k[3]);
#endif
	}

	v[0]=y; v[1]=z;

#ifdef DEBUG
	printf( "= %.8lX %.8lX\n", v[0], v[1] );
#endif

	hton_ul(k,4);
	hton_ul(v,2);
}

void tea_dec( uint32_t *v, uint32_t *k )
{
	uint32_t y, z;
	uint32_t sum=DELTA*ROUNDS;
	uint8_t n=ROUNDS;

	hton_ul(k,4);
	hton_ul(v,2);

#ifdef DEBUG
	printf( "tea_dec( %.8lX %.8lX, %.8lX %.8lX %.8lX %.8lX )", v[0], v[1],
		k[0], k[1], k[2], k[3] );
#endif

	y=v[0]; z=v[1];

	while(n-->0) {
#if defined(TEA_SMALL)
		z -= tea_func( &y, &sum, &(k[2]) );
		y -= tea_func( &z, &sum, &(k[0]) );
#else
		z -= ((y<<4)+k[2]) ^ (y+sum) ^ ((y>>5)+k[3]);
		y -= ((z<<4)+k[0]) ^ (z+sum) ^ ((z>>5)+k[1]);
#endif
		sum -= DELTA;
	}

	v[0]=y; v[1]=z;

#ifdef DEBUG
	printf( "= %.8lX %.8lX\n", v[0], v[1] );
#endif

	hton_ul(k,4);
	hton_ul(v,2);
}

#endif /* CONF_WITH_CRYPT_ALGO==0 */


