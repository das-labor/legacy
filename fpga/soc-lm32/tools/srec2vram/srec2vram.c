#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

uint32_t rambase;
uint32_t ramsize;
uint8_t  *ram;

#define HAVE_STRNDUP
#ifndef HAVE_STRNDUP
char* strndup(const char* s, size_t n) {
       char* ret = malloc(n + 1);
       if (ret == NULL) return(ret);
       ret[n] = '\0';
       return(memcpy(ret, s, n));
}
#endif

void help()
{
	printf( "\nUsage: srec2vram <srect-file> <bram-base> <bram-size>\n\n" );
	printf( " <bram-base> \t \n" );
	printf( " <bram-size> \t \n\n" );
};


uint32_t parsehex(char *line, int start, int len)
{
	char *str;
	uint32_t val;

	str = strndup(line+start, len);
	sscanf(str, "%x", &val);
	free(str);

//	fprintf( stderr, "parsehex: %x\n", val );

	return val;
}

void parseline(char *line, int start, int bytes, uint32_t addr)
{
	int i;
	uint8_t val;

//	fprintf( stderr, "parsline: [0x%08x, 0x%08x)\n", addr, addr+bytes );

	for(i=0; i<bytes; i++) {
		if ( (rambase <= addr+i) && (addr+i < rambase+ramsize) ) {
			val = (uint8_t)parsehex(line, start+(i*2), 2);
			ram[addr-rambase+i] = val;
		}
	}
}

#define LINESIZE 200

int main(int argc, char **argv)
{
	int err;
	FILE *infile;

	if (argc != 4) {
		help();
		exit(1);
	}

	//
	err = sscanf(argv[2], "%x", &rambase);
	if (err != 1) {
		printf( "ERROR: Could not parse <bram-base> (%s)\n", argv[2] );
		exit(1);
	}
		
	
	err = sscanf(argv[3], "%i", &ramsize);
	if (err != 1) {
		printf( "ERROR: Could not parse <bram-size> (%s)\n", argv[3] );
		exit(1);
	}

	ram = malloc( ramsize );
	fprintf( stderr, "Extracting [%08x,%08x) (size=0x%x)\n",
		rambase, rambase+ramsize, ramsize );
		
	// Open SREC file
	infile = fopen( argv[1], "r" );
	if (infile == NULL) {
		perror( "Could not open infile" );
		exit(1);
	}

	// Read SREC file
	while( !feof(infile) ) {
		char errmsg[] = "Could not parse '%s'\n";
		char line[LINESIZE];
		uint32_t addr;
		int len;

		
		fgets(line, LINESIZE, infile);
		
		// Is this a sane SREC line?
		if (strlen(line) < 4) {
			fprintf( stderr, errmsg, line );
			continue;
		}

		if (line[0] != 'S') {
			fprintf( stderr, errmsg, line );
			continue;
		}
		
		// record length
		len = parsehex(line, 2, 2);

		switch ( line[1] ) {
		case '0':
			continue;
		case '1':             // data, 2-byte address
			addr = parsehex(line, 4, 4);
			parseline(line, 8, len-3, addr);
			break;
		case '2':             // data, 3-byte address
			addr = parsehex(line, 4, 6);
			parseline(line, 10, len-4, addr);
			break;
		case '3':             // data, 4-byte address
			addr = parsehex(line, 4, 8);
			parseline(line, 12, len-5, addr);
			break;
		case '7':             // 4-byte start address
			break;
		case '8':             // 3-byte start address
			break;
		case '9':             // 2-byte start address
			break;
		default:
			fprintf( stderr, errmsg, line );

		}
	}

	fclose( infile );

	// Write output
	int i;

	for(i = 0; i < ramsize; i+=4) {
		printf( "%02x%02x%02x%02x\n", ram[i+0], ram[i+1], ram[i+2], ram[i+3] );
	}

	return 0;
}

