#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <FTGL/ftgl.h>
#include "font.h"
#include "infowin.h"
static char *info_txt = NULL;
static uint32_t txt_len = 0;

float event_fade (uint8_t in_kick)
{
	static struct timeval last_ev = {0,0};
	struct timeval tv;
	long cus, lus;
	static uint8_t state = 0;
	
	gettimeofday (&tv, NULL);

	if (in_kick)
	{
		memcpy (&last_ev, &tv, sizeof(struct timeval));
		state = 1;
	}

	/* initialization without a kick (unlikely) */
	if (last_ev.tv_sec == 0)
	{
		gettimeofday (&last_ev, NULL);
		return 0.0f;
	}
	
	if (!state)
		return 0.0f;
	
	cus = ((tv.tv_sec - last_ev.tv_sec) * 1000000) + tv.tv_usec;
	lus = last_ev.tv_usec;

	if (cus < lus)
		lus = 1000000 - lus;
	
	cus -= lus;
	
	switch (state)
	{
		case 1:  /* fade in */
			if (cus <= (EV_FADETIME * 1000000))
				return (float) (((double) cus) / 
					(((double) EV_FADETIME) * 1000000.0f));
			
			/* intentional fallthrough */
			state = 2;

		case 2:
			if (cus < ((EV_TIMEOUT + EV_FADETIME) * 1000000))
				return 1.0f;

			state = 3;
			/* intentional fallthrough */

		case 3:
			if (cus <= ((EV_TIMEOUT + (2*EV_FADETIME)) * 1000000))
				return (float)
					(1.0f) - ((float) ((cus - ((EV_TIMEOUT + EV_FADETIME) * 1000000))) / ((float) (EV_FADETIME *1000000)));

			/* intentional fallthrough */
		default:
			state = 0;
			return 0.0f;
	}
}

void print_txt (char *in_txt)
{
	if (info_txt == NULL) /* make sure the buffer is zero'ed even on first run */
		txt_len = strlen(in_txt);
	
	info_txt = realloc (info_txt, (strlen(in_txt) +1));
	memset (info_txt, '\0', strlen(in_txt) +1);
	txt_len = strlen(in_txt);
	memcpy (info_txt, in_txt, strlen(in_txt));
	event_fade (1); /* kick the event fade timer */
}

void ftgl_print ()
{
	static FTGLlayout *mylayout;
	static FTGLfont *font = NULL;
	static uint8_t init_done = 0;

	if (info_txt == NULL)
		return;
	
	if (!init_done)
	{
	//	if (font != NULL)
	//		ftglDestroyFont(font);

		mylayout = ftglCreateSimpleLayout();

		ftglSetLayoutLineLength (mylayout, 700.0f);
		font = ftglCreatePolygonFont("./fonts/zekton_free.ttf");
		if (!font)
		{
			font = ftglCreatePolygonFont(
				"/usr/share/fonts/truetype/msttcorefonts/Arial.ttf");
		}

		ftglSetFontFaceSize(font, 96, 4);
		ftglSetLayoutFont (mylayout, font);
		init_done = 1;
	}
	/*
	somebody forgot to implement this function - therefore it's disabled... *sigh*
	ftglGetLayoutBBox (mylayout, mybuf, bounds);
	*/
	
	ftglRenderLayout (mylayout, info_txt, FTGL_RENDER_FRONT);
}


void draw_infowin()
{
	double ev;

	ev = event_fade (0);

	if (ev == 0.0f)
		return; 

	glLoadIdentity ();
	glTranslatef (0.0f, 0.0f, -8.0f);
	glColor4f (1.0f, 0.4f, 0.0f, 0.5f);
	glBegin (GL_QUADS);
		glVertex3f (3.0f, -5.0f,  0.0f);
		glVertex3f (10.0f, -5.0f, 0.0f);
		glColor4f (0.5f, 0.2f, 0.0f, 0.5f);
		glVertex3f (10.0f,  5.0f, 0.0f);
		glVertex3f (3.0f,  5.0f,  0.0f);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, ev);

	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity ();
	glScaled (0.001f * ev, 0.001f * ev, 0.1f *ev);
	glTranslatef (410.0f, 900.0f, 0.0f);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	ftgl_print();
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
}
