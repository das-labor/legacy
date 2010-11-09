#include <stdint.h>
#include <stdlib.h>
#include <GL/gl.h>
#include "event.h"

GLfloat *get_event_coords (loc_names in_name)
{
	uint_fast16_t i;

	for (i=0;i<num_locations;i++)
	{
		if (locations[i] == in_name)
		{
			/* XXX sizeof correct? */
			return &(locations[i]) + sizeof(loc_names);
		}
	}

	return NULL;
}

void set_eventlights (uint8_t in_color, loc_names in_location)
{
	const GLfloat lpos[4] = {0.0f, 0.0f, 17.0f, 1.0f};
	const GLfloat lamb[4] = {0.8f, 0.0f, 0.0f, 1.0f};
	const GLfloat ldiff[4] = {0.8f, 0.0f, 0.0f, 1.0f};
	const GLfloat *ldir;

	ldir = get_event_coords (in_name);

	if (ldir == NULL)
		return;
	
	glBegin(GL_LINES);
		glVertex3fv (lpos);
		glVertex3fv (ldir);
	glEnd();

	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, ldir);

	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 16.0f);
	glLight f(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0f);

	glLightfv (GL_LIGHT0, GL_AMBIENT,  lamb);
	glLightfv (GL_LIGHT0, GL_DIFFUSE,  ldiff);
	glLightfv (GL_LIGHT0, GL_POSITION, lpos);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}
