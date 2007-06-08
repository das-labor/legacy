#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define BLOCKSIZE 64
#define UDELAY 1

void help()
{
	printf( "uploader <binfile> <ttySX>\n" );
};

void writeint(FILE *f, uint32_t i)
{
	usleep(UDELAY);
	fputc( (i & 0xff000000) >> 24, f);
	fflush( f );
	usleep(UDELAY);
	fputc( (i & 0x00ff0000) >> 16, f);
	fflush( f );
	usleep(UDELAY);
	fputc( (i & 0x0000ff00) >>  8, f);
	fflush( f );
	usleep(UDELAY);
	fputc( (i & 0x000000ff) >>  0, f);
	fflush( f );
};

int main(int argc, char **argv)
{
	char buf[101];
	uint32_t blockstart, checksum;
	int c, i, filesize;
	FILE *infile;
	FILE *tty;

	if (argc != 3) {
		help();
		exit(1);
	}

	infile = fopen( argv[1], "r" );
	if (infile == NULL) {
		perror( "Could not open infile" );
		exit(1);
	}

	tty = fopen( argv[2], "r+" );
	if (tty == NULL) {
		perror( "Could not open tty" );
		exit(1);
	}

	fseek( infile, 0, SEEK_END );
	filesize = ftell( infile );
	fseek( infile, 0, SEEK_SET );

	blockstart = 0;
	while(blockstart <= filesize) {
		char buf[101];
		uint32_t sstart, ssize, schecksum;

		// Upload single block
		fseek( infile, blockstart, SEEK_SET );
		fputc( 'u', tty );
		writeint( tty, 0xb0000000 + blockstart);
		writeint( tty, BLOCKSIZE );
		checksum = 0;
		for( i=0; i<BLOCKSIZE; i++ ) {
			c=fgetc(infile);
			checksum += (unsigned char)c;
			//if (!i%1024) usleep(UDELAY);
			fputc( c, tty);
			fflush( tty );
		}
		
		fgets( buf, 100, tty );
		sscanf( buf, "u:%x:%x:%x.", &sstart, &ssize, &schecksum );
		printf( "Frame: 0x%08x - 0x%08x (checksum= 0x%08x/0x%08x)\n", sstart, sstart+ssize, schecksum, checksum );
		if (schecksum == checksum) {
			// Next block
			blockstart += BLOCKSIZE;
		} else {
			printf( "*** RETRANSMIT ***\n" );
		}
	}

	fputc( 'g', tty );
	writeint( tty, 0xb0000000 );

	return 0;
}
