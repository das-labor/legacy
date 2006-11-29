#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

static int width;
static uint32_t state;


void write_header(FILE *f)
{
	time_t t;
	int i;

	t = time(NULL);
	printf( "$date\n" );
	printf( "  %s", ctime(&t) );
	printf( "$end\n" );
	printf( "$version\n" );
	printf( "  spikela\n" );
	printf( "$end\n" );
	printf( "$timescale\n" );
	printf( "$  1 ns\n" );
	printf( "$end\n" );

	for(i=0; i<width; i++)
		printf( "$var reg 1 %c Chan%d $end\n", 'A'+i, i );

	printf( "$enddefinitions $end\n" );
}


void write_all(FILE *f, uint32_t t, uint32_t newstate) 
{
	int i;
	printf( "#%d\n", t );

	for(i=0; i<width; i++)
		printf( "%d%c\n", (newstate >> i) & 1, 'A'+i );

	state = newstate;
}

void write_changes(FILE *f, uint32_t t, uint32_t newstate)
{
	uint32_t diff;
	int i;

	diff = newstate ^ state;
	if (diff)
		printf( "#%d\n", t );

	for(i=0; i<width; i++) {
		if ( (diff >> i) & 1 ) 
			printf( "%d%c\n", (newstate >> i) & 1, 'A'+i );
	}

	state = newstate;
}



int main(int argc, char **argv)
{
	int t, i, length, c;
	FILE *f;

	width = 4;

	// Open Source
	f = fopen(argv[1], "r");
	if (f == NULL) {
		perror( "Could not open scope source file" );
		exit(1);
	}

	c = fgetc(f);
	if ( c != 'S' ) {
		fprintf( stderr, "Scope file format error\n" );
		exit(1);
	}
	width = fgetc(f);
	length = fgetc(f); length <<= 8;
	length += fgetc(f);
	fprintf( stderr, "Reading 0x%x bytes Scope Data...", length );
	
	write_header(f);
	write_all(f, 0, 0);

	t = 1;
	for( i=0; i<length; i++ ) {
		c = fgetc(f);
		write_changes(f, t*20, (c & 0xf0) >> 4 );
		t++;
		write_changes(f, t*20, (c & 0x0f) >> 0 );
		t++;
	}
	fprintf( stderr, "DONE.\n" );

	c = fgetc(f);
	if ( c != 'E' ) {
		fprintf( stderr, "Scope file format error\n" );
	}

	fclose(f);
}

