#include "zImage.h"

#define N		 4096	/* size of ring buffer */
#define F		   18	/* upper limit for match_length */
#define THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */
#define NIL			N	/* index for root of binary search trees */
#define EOF        -1

unsigned char text_buf[N + F - 1];	/* ring buffer of size */

#define RAMSTART (unsigned int *) (0x40000000 + 1024*(512 - 16))

char * pIn;
char * pInEnd;
char * pOut;

int getc() {
	int c = *pIn++;
	if (pIn >= pInEnd)
		return EOF;
	else
		return c;
}

void putc(int in) {
	*pOut++ = in;
}

void Decode(void)	/* Just the reverse of Encode(). */
{
	int  i, j, k, r, c;
	unsigned int  flags;
	
	for (i = 0; i < N - F; i++) text_buf[i] = ' ';
	r = N - F;  flags = 0;
	for ( ; ; ) {
		if (((flags >>= 1) & 256) == 0) {
			if ((c = getc()) == EOF) break;
			flags = c | 0xff00;		/* uses higher byte cleverly */
		}							/* to count eight */
		if (flags & 1) {
			if ((c = getc()) == EOF) break;
			putc(c);  text_buf[r++] = c;  r &= (N - 1);
		} else {
			if ((i = getc()) == EOF) break;
			if ((j = getc()) == EOF) break;
			i |= ((j & 0xf0) << 4);  j = (j & 0x0f) + THRESHOLD;
			for (k = 0; k <= j; k++) {
				c = text_buf[(i + k) & (N - 1)];
				putc(c);  text_buf[r++] = c;  r &= (N - 1);
			}
		}
	}
}

void jump(int* addr);

int main() {
	pIn    = zImage;
	pInEnd = zImage + Z_IMAGE_SIZE;
	pOut   = (char *) RAMSTART;
	
	Decode();
	jump(RAMSTART);
	
	return 0;
}
