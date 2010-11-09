#include <stdint.h>
#include <stdlib.h>
#include <GL/gl.h>


enum loc_names {
	basement_window,
	basement_smokers_room,
	basement_workshop_room,
	basement_storage_room,
	stairs,
	lecture_room,
	lecture_windows,
	lecture_window0,
	lecture_window1,
	lecture_window2,
	lecture_window3,
	lecture_window4,
	lecture_window5,
	lecture_window6,
	lecture_window7,
	toilet0,
	toilet1,
	lounge_room,
	kitchen_room,
	lounge_windows,
	lounge_window0,
	lounge_window1,
	lounge_window2,
	lounge_window3,
	workshop_room,
	workshop_windows,
	workshop_window0,
	workshop_window1,
	workshop_window2,
	workshop_window3
};

typedef struct {
	loc_names name;
	GLfloat x;
	GLfloat y;
	GLfloat z;
} location_v;

const location_v locations[] =
{
	{basement_window, 7.0f, 0.5f, -1.0f},
	{basement_smokers_room, 6.5f, 0.5f, -1.0f},
	{basement_workshop_room, 5.8f, 0.5f, -1.0f}
};

const num_locations = (sizeof(locations) / sizeof(location_v));


void set_eventlights (uint8_t, uint16_t);
float *get_event_coords (loc_names in_name);
