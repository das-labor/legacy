#include <stdlib.h>
#include <GL/gl.h>
#pragma once

void show_lines ();
void effect_intro ();
void show_coordsys ();
void show_triangle ();
void draw_landscape (GLuint);
void drawfog();
void effect_handler (uint8_t in_cmd);
void show_repeatexample();

#define EF_SHOW 0x00
#define EF_NEXT 0x01
#define EF_LAST 0x02
#define EF_SWITCH 0x03
#define EF_DISABLE 0x04
#define EF_ENABLE 0x05
#define EF_TICK 0x06
