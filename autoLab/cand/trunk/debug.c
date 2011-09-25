#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "debug.h"

int debug_level  = 0;
int debug_syslog = 0;
FILE *debugFP;

void init_debug(char* debugfile)
{
	if (debugfile) {
		if ((debugFP = fopen(debugfile,"a")) == NULL)
		{
			printf("Failed to open Debuglogfile\n");
			exit(EXIT_FAILURE);
		}
	}
	else
		debugFP = stderr;
}

void debug_close() {
	if (debugFP != NULL)
		fclose(debugFP);
}

void debug( int level, char *format, ... )
{
	va_list ap;

	if (debug_level < level) {
		return;
	}

	if (level == 0)
		fprintf(debugFP, "ERROR: ");

	va_start(ap, format);
	vfprintf(debugFP, format, ap);
	fprintf(debugFP, "\n");
	va_end(ap);
}

void debug_perror( int level, char *format, ... )
{
	va_list ap;

	if (debug_level < level)
		return;

	if (level == 0)
		fprintf(debugFP, "ERROR: ");

	//debug
	fprintf(debugFP, "1: %i\n", level);
	fprintf(debugFP, "2: %u, %s\n", format, format);

	va_start(ap, format);
	vfprintf(debugFP, format, ap);
	fprintf(debugFP, " (%s)\n", strerror(errno) );
	va_end(ap);
}

void debug_assert( int test, char *format, ... )
{
	if (test)
		return;

	va_list ap;

	va_start(ap, format);
	fprintf(debugFP, "ERROR: ");
	vfprintf(debugFP, format, ap);
	fprintf(debugFP, " (%s)\n", strerror(errno) );
	va_end(ap);

	exit(1);
}

void debug_assert2(char *format, ... )
{
	va_list ap;

	va_start(ap, format);
	fprintf(debugFP, "ERROR: ");
	vfprintf(debugFP, format, ap);
	fprintf(debugFP, " (%s)\n", strerror(errno) );
	va_end(ap);

	exit(1);
}

