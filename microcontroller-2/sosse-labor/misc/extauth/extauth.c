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

#include <stdio.h>

#define DELTA   0x9E3779B9
#define ROUNDS  32

void tea_enc( unsigned long *const v, const unsigned long *const k )
{
	unsigned long	y=v[0], z=v[1], sum=0, n=ROUNDS;

	while(n-->0) {
		sum += DELTA;
		y += ((z<<4)+k[0]) ^ (z+sum) ^ ((z>>5)+k[1]);
		z += ((y<<4)+k[2]) ^ (y+sum) ^ ((y>>5)+k[3]);
	}

	v[0]=y; v[1]=z;
}

int main( int argc, char *argv[] )
{
	unsigned int ibuf[8];
	unsigned long block[2], key[4] = {
			0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF
//			0x67452301, 0xEFCDAB89, 0x67452301, 0xEFCDAB89
		};
	int ret, i;

	if( argc!=2 ) return 1;

	ret = sscanf( argv[1], "%2X%2X%2X%2X%2X%2X%2X%2X", &ibuf[0],
		&ibuf[1], &ibuf[2], &ibuf[3], &ibuf[4], &ibuf[5], &ibuf[6], &ibuf[7] );
	if( ret!=8 ) return 2;

	block[0] =
		((ibuf[0]&0xFF)<<24) |
		((ibuf[1]&0xFF)<<16) |
		((ibuf[2]&0xFF)<<8) |
		((ibuf[3]&0xFF));
	block[1] =
		((ibuf[4]&0xFF)<<24) |
		((ibuf[5]&0xFF)<<16) |
		((ibuf[6]&0xFF)<<8) |
		((ibuf[7]&0xFF));

	printf( "Key: %.8X %.8X %.8X %.8X\n", key[0], key[1], key[2], key[3] );
	printf( "Challenge: %.8X %.8X\n", block[0], block[1] );
	tea_enc( block, key );
	printf( "Response: %.8X %.8X\n", block[0], block[1] );


	return 0;
}

