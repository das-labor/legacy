#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <SDL.h>

#include "font.h"

#define NUM_FONTS 8

static GLuint bases[NUM_FONTS]; /* Base Display List For The Font Set */
static Font fids[NUM_FONTS];
static uint16_t sizes[NUM_FONTS];
char fnames[NUM_FONTS][32];
static uint8_t fidx = 0;

/* function to recover memory form our list of characters */
GLvoid KillFont( GLvoid )
{
	glDeleteLists( bases[fidx], 96 );

	return;
}

/* function to build our font list */
GLvoid buildFont(uint16_t in_size )
{
	Display *dpy;          /* Our current X display */
	XFontStruct *fontInfo; /* Our font info */
	char fname[80];
	uint8_t i;
	static GLuint mybase;

//	sprintf(fname, "-*-impact-*-*-*-*-*-%u-*-*-*-*-*-*", in_size);
    	sprintf(fname, "-adobe-helvetica-medium-r-normal--%u-*-*-*-p-*-iso8859-1", in_size);

	/* Sotrage for 96 characters */
	mybase = glGenLists( 96 );

	/* Get our current display long enough to get the fonts */
	dpy = XOpenDisplay( NULL );

	/* Get the font information */
	fontInfo = XLoadQueryFont (dpy, fname);

	/* If the above font didn't exist try one that should */
	if ( fontInfo == NULL )
	{
		fontInfo = XLoadQueryFont( dpy, "fixed" );
		/* If that font doesn't exist, something is wrong */
		if ( fontInfo == NULL )
		{
			fprintf( stderr, "no X font available?\n" );
			exit(-1);
		}
	}

	/* generate the list */
	glXUseXFont( fontInfo->fid, 32, 96, mybase);

	/* Recover some memory */
	XFreeFont( dpy, fontInfo );

	/* close the display now that we're done with it */
	XCloseDisplay( dpy );

	for (i=0;i<NUM_FONTS;i++)
	{
		if (sizes[i] == 0xffff)
		{
			fids[i] = fontInfo->fid;
			sizes[i] = in_size;
			bases[i] = mybase;
			strncpy (fnames[i], fname, strlen(fname));
			return;
		}
	}

	return;
}

void select_font (uint16_t in_size)
{
	static uint8_t init_done = 0;
	uint8_t i = 0;
	
	if (!init_done)
	{
		KillFont ();
		init_done = 1;
		for (i=0;i<NUM_FONTS;i++)
			sizes[i] = 0xffff;
	}
	
	for (i=0; i<NUM_FONTS; i++)
	{
		if (sizes[i] == in_size)
		{
			fidx = i;
			glXUseXFont(fids[i], 32, 96, bases[i]);
			return;
		}

		/* end of list reached */
		if (sizes[i] == 0xffff)
			break;
	}
	buildFont ( in_size );
}


/* Print our GL text to the screen */
GLvoid glPrint( const char *fmt, ... )
{
	char text[256]; /* Holds our string */
	va_list ap;     /* Pointer to our list of elements */

	/* If there's no text, do nothing */
	if ( fmt == NULL )
		return;

	/* Parses The String For Variables */
	va_start( ap, fmt );
		/* Converts Symbols To Actual Numbers */
		vsprintf( text, fmt, ap );
	va_end( ap );

	/* Pushes the Display List Bits */
	glPushAttrib( GL_LIST_BIT );

	/* Sets base character to 32 */
	glListBase( bases[fidx] - 32 );

	/* Draws the text */
	glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );

	/* Pops the Display List Bits */
	glPopAttrib( );
}
