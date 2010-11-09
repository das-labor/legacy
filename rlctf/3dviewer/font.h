#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <SDL.h>
#include <stdint.h>

#pragma once

GLvoid buildFont(uint16_t);
GLvoid glPrint( const char *fmt, ... );
void select_font (uint16_t in_id);
