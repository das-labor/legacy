/*
    shapes.c
    Mason Woo, 1999
    with modification of code by Nate Robins, 1997

    Tool for teaching about OpenGL primitives.
    
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "glm.h"


typedef struct _cell {
    int id;
    int x, y;
    float min, max;
    float value;
    float step;
    char* info;
    char* format;
} cell;


#define MESSAGEY 455


cell lines[2*3+4*2] = {
    { 1, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 2, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 3, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 4, 120, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 5, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 6, 120, 120, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 7, 180, 120, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 8, 120, 150, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 9, 180, 150, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 10, 240, 150, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 11, 120, 180, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 12, 180, 180, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 13, 120, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 14, 180, 210, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell shadeFour[4*3+4*2] = {
    { 15, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 16, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 17, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 18, 120, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 19, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 20, 120, 120, 0.0f, 1.0f, 0.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 21, 180, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 22, 240, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 23, 120, 150, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 24, 180, 150, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 25, 120, 180, 0.0f, 1.0f, 0.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 26, 180, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 27, 240, 180, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 28, 120, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 29, 180, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 30, 120, 240, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 31, 180, 240, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 32, 240, 240, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 33, 120, 270, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 34, 180, 270, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell flatFour[1*3+4*2] = {
    { 35, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 36, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 37, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 38, 120, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 39, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 40, 120, 120, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 41, 180, 120, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 42, 120, 150, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 43, 180, 150, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 44, 120, 180, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 45, 180, 180, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell shadeThree[3*3+3*2] = {
    { 46, 120, 60, 0.0f, 1.0f, 0.5f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 47, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 48, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 49, 120, 90, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 50, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 51, 120, 120, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 52, 180, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 53, 240, 120, 0.0f, 1.0f, 0.5f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 54, 120, 150, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 55, 180, 150, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 56, 120, 180, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 57, 180, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 58, 240, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 59, 120, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 60, 180, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell flatThree[1*3+3*2] = {
    { 61, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 62, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 63, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 64, 120, 90, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 65, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 66, 120, 120, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 67, 180, 120, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 68, 120, 150, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 69, 180, 150, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell shadeQStrip[6*3+6*2] = {
    { 70, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 71, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 72, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 73, 120, 90, 0.0f, 200.0f, 25.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 74, 180, 90, 0.0f, 200.0f, 25.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 75, 120, 120, 0.0f, 1.0f, 0.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 76, 180, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 77, 240, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 78, 120, 150, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 79, 180, 150, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 80, 120, 180, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 81, 180, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 82, 240, 180, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 83, 120, 210, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 84, 180, 210, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 85, 120, 240, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 86, 180, 240, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 87, 240, 240, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 88, 120, 270, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 89, 180, 270, 0.0f, 200.0f, 75.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 90, 120, 300, 0.0f, 1.0f, 0.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 91, 180, 300, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 92, 240, 300, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 93, 120, 330, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 94, 180, 330, 0.0f, 200.0f, 25.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 95, 120, 360, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 96, 180, 360, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 97, 240, 360, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 98, 120, 390, 0.0f, 200.0f, 175.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 99, 180, 390, 0.0f, 200.0f, 125.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell shadePoints[6*3+6*2] = {
    { 100, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 101, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 102, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 103, 120, 90, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 104, 180, 90, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 105, 120, 120, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 106, 180, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 107, 240, 120, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 108, 120, 150, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 109, 180, 150, 0.0f, 200.0f, 175.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 110, 120, 180, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 111, 180, 180, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 112, 240, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 113, 120, 210, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 114, 180, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 115, 120, 240, 0.0f, 1.0f, 0.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 116, 180, 240, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 117, 240, 240, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 118, 120, 270, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 119, 180, 270, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 120, 120, 300, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 121, 180, 300, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 122, 240, 300, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 123, 120, 330, 0.0f, 200.0f, 75.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 124, 180, 330, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 125, 120, 360, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 126, 180, 360, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 127, 240, 360, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 128, 120, 390, 0.0f, 200.0f, 175.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 129, 180, 390, 0.0f, 200.0f, 75.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell tStrip[4*3+4*2] = {
    { 130, 120, 60, 0.0f, 1.0f, 0.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 131, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 132, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 133, 120, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 134, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 135, 120, 120, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 136, 180, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 137, 240, 120, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 138, 120, 150, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 139, 180, 150, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 140, 120, 180, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 141, 180, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 142, 240, 180, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 143, 120, 210, 0.0f, 200.0f, 175.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 144, 180, 210, 0.0f, 200.0f, 25.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 145, 120, 240, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 146, 180, 240, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 147, 240, 240, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 148, 120, 270, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 149, 180, 270, 0.0f, 200.0f, 175.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell tFan[4*3+4*2] = {
    { 150, 120, 60, 0.0f, 1.0f, 0.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 151, 180, 60, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 152, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 153, 120, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 154, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 155, 120, 120, 0.0f, 1.0f, 0.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 156, 180, 120, 0.0f, 1.0f, 0.5f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 157, 240, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 158, 120, 150, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 159, 180, 150, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 160, 120, 180, 0.0f, 1.0f, 0.5f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 161, 180, 180, 0.0f, 1.0f, 0.5f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 162, 240, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 163, 120, 210, 0.0f, 200.0f, 175.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 164, 180, 210, 0.0f, 200.0f, 175.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 165, 120, 240, 0.0f, 1.0f, 0.5f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 166, 180, 240, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 167, 240, 240, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 168, 120, 270, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 169, 180, 270, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell shadeQuad[4*3+4*2] = {
    { 170, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 171, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 172, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 173, 120, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 174, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 175, 120, 120, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 176, 180, 120, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 177, 240, 120, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 178, 120, 150, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 179, 180, 150, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 180, 120, 180, 0.0f, 1.0f, 1.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 181, 180, 180, 0.0f, 1.0f, 0.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 182, 240, 180, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 183, 120, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 184, 180, 210, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    
    { 185, 120, 240, 0.0f, 1.0f, 0.0f, 0.005f,
    "Specifies red component of a color.", "%.2f" },
    { 186, 180, 240, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 187, 240, 240, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 188, 120, 270, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 189, 180, 270, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

cell flatQuad[1*3+4*2] = {
    { 190, 120, 60, 0.0f, 1.0f, 1.0f, 0.005f,
        "Specifies red component of a color.", "%.2f" },
    { 191, 180, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies green component of a color.", "%.2f" },
    { 192, 240, 60, 0.0f, 1.0f, 1.0f, 0.005f, 
    "Specifies blue component of a color.", "%.2f" },
    { 193, 120, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 194, 180, 90, 0.0f, 200.0f, 50.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 195, 120, 120, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 196, 180, 120, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 197, 120, 150, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 198, 180, 150, 0.0f, 200.0f, 200.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" },
    { 199, 120, 180, 0.0f, 200.0f, 150.0f, 1.0f, 
    "Specifies x component of a vertex.", "%.1f" },
    { 200, 180, 180, 0.0f, 200.0f, 100.0f, 1.0f, 
    "Specifies y component of a vertex.", "%.1f" }
};

GLboolean clear = GL_TRUE;
GLenum mode = GL_LINES;
int shaded = GL_FALSE;
int bigVertices = GL_FALSE;
int outlined = GL_FALSE;

void redisplay_all(void);
void screen_menu(int);
GLuint window, screen, command;
GLuint sub_width = 256, sub_height = 512;
GLint selection = 0;
int sm;

GLuint circleList;
#define _PI 3.14159265

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

static float linesInit[] = {1.0, 1.0, 1.0, 
50.0, 50.0, 100.0, 100.0,
1.0, 1.0, 1.0, 
150.0, 100.0, 200.0, 150.0};

static float shadeFourInit[] = 
{1.0, 1.0, 1.0, 50.0, 50.0, 
0.0, 1.0, 1.0, 100.0, 150.0,
0.0, 1.0, 0.0, 200.0, 200.0, 
1.0, 1.0, 0.0, 150.0, 100.0};

static float flatFourInit[] = 
{1.0, 1.0, 1.0, 
50.0, 50.0, 
100.0, 150.0,
200.0, 200.0, 
150.0, 100.0};

static float flatQuadInit[] = 
{1.0, 1.0, 1.0, 
50.0, 50.0, 
100.0, 150.0,
200.0, 200.0, 
150.0, 100.0};

static float shadeThreeInit[] = 
{0.5, 1.0, 1.0, 100.0, 50.0, 
1.0, 1.0, 0.5, 50.0, 150.0,
1.0, 1.0, 1.0, 200.0, 200.0};

static float flatThreeInit[] = 
{1.0, 1.0, 1.0, 
100.0, 50.0, 
50.0, 150.0,
200.0, 200.0};

static float shadeQStripInit[] = 
{1.0, 1.0, 1.0, 25.0, 25.0, 
0.0, 1.0, 1.0, 50.0, 100.0,
1.0, 1.0, 0.0, 100.0, 50.0, 
1.0, 1.0, 0.0, 100.0, 75.0,
0.0, 1.0, 0.0, 150.0, 25.0, 
1.0, 0.0, 0.0, 175.0, 125.0};

static float tFanInit[] = 
{0.0, 0.0, 1.0, 50.0, 50.0, 
0.0, 0.5, 1.0, 100.0, 150.0,
0.5, 0.5, 1.0, 175.0, 175.0, 
0.5, 0.0, 1.0, 200.0, 100.0};

static float tStripInit[] = 
{0.0, 1.0, 1.0, 50.0, 50.0, 
1.0, 1.0, 0.0, 100.0, 150.0,
1.0, 1.0, 0.0, 175.0, 25.0, 
1.0, 0.0, 0.0, 150.0, 175.0};

static float shadePointsInit[] = 
{1.0, 1.0, 1.0, 100.0, 100.0, 
1.0, 1.0, 0.0, 200.0, 175.0,
1.0, 0.0, 1.0, 50.0, 200.0, 
0.0, 1.0, 1.0, 150.0, 150.0,
1.0, 1.0, 1.0, 75.0, 50.0, 
1.0, 1.0, 0.0, 175.0, 75.0};

static float shadeQuadInit[] = 
{1.0, 1.0, 1.0, 50.0, 50.0,
1.0, 1.0, 0.0, 100.0, 150.0,
1.0, 0.0, 1.0, 200.0, 200.0, 
0.0, 1.0, 1.0, 150.0, 100.0};

void
setfont(char* name, int size)
{
    font_style = GLUT_BITMAP_HELVETICA_10;
    if (strcmp(name, "helvetica") == 0) {
        if (size == 12) 
            font_style = GLUT_BITMAP_HELVETICA_12;
        else if (size == 18)
            font_style = GLUT_BITMAP_HELVETICA_18;
    } else if (strcmp(name, "times roman") == 0) {
        font_style = GLUT_BITMAP_TIMES_ROMAN_10;
        if (size == 24)
            font_style = GLUT_BITMAP_TIMES_ROMAN_24;
    } else if (strcmp(name, "8x13") == 0) {
        font_style = GLUT_BITMAP_8_BY_13;
    } else if (strcmp(name, "9x15") == 0) {
        font_style = GLUT_BITMAP_9_BY_15;
    }
}

void 
drawstr(GLuint x, GLuint y, char* format, ...)
{
    va_list args;
    char buffer[255], *s;
    
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    glRasterPos2i(x, y);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(font_style, *s);
}

void
cell_draw(cell* cell)
{
    glColor3ub(0, 255, 128);
    if (selection == cell->id) {
        glColor3ub(255, 255, 0);
        drawstr(10, MESSAGEY, cell->info);
        glColor3ub(255, 0, 0);
    }
    
    drawstr(cell->x, cell->y, cell->format, cell->value);
}

int
cell_hit(cell* cell, int x, int y)
{
    if (x > cell->x && x < cell->x + 60 &&
        y > cell->y-20 && y < cell->y+10)
        return cell->id;
    return 0;
}

void
cell_update(cell* cell, int update)
{
    if (selection != cell->id)
        return;
    
    cell->value += update * cell->step;
    
    if (cell->value < cell->min)
        cell->value = cell->min;
    else if (cell->value > cell->max) 
        cell->value = cell->max;
}

void
cell_vector(float* dst, cell* cell, int num)
{
    while (--num >= 0)
        dst[num] = cell[num].value;
}

void createCircleList (void) {
    GLUquadricObj *qobj;
    float radius = 4.0f;
    int slices = 11;
    int i;
    
    circleList = glGenLists(1);
    qobj = gluNewQuadric();
    glNewList (circleList, GL_COMPILE);
    glBegin (GL_TRIANGLE_FAN);
    glVertex2f (0.0f, 0.0f);
    for (i = 0; i <= slices; i++) {
        glVertex2f (radius * cos ((2.0 * _PI) * (float) i/(float) slices),
            radius * sin ((2.0 * _PI) * (float) i/(float) slices));
    }
    glEnd();
    glEndList();
}

void drawCircle (GLfloat coords[]) {
    glPushMatrix();
    glTranslatef(coords[0], coords[1], 0.0f);
    glCallList(circleList);
    glPopMatrix();
}

void
drawmodel(void)
{
    GLfloat vals[50];
    
    if ((mode == GL_LINES) && !shaded ) {
        cell_vector (vals, lines, sizeof(linesInit)/sizeof(float));
        glBegin (GL_LINES);
        glColor3fv (&vals[0]);
        glVertex2fv (&vals[3]);
        glVertex2fv (&vals[5]);
        glColor3fv (&vals[7]);
        glVertex2fv (&vals[10]);
        glVertex2fv (&vals[12]);
        glEnd();
        if (bigVertices) {
            glColor3fv (&vals[0]);
            drawCircle (&vals[3]);
            drawCircle (&vals[5]);
            glColor3fv (&vals[7]);
            drawCircle (&vals[10]);
            drawCircle (&vals[12]);
        }
    }
    else if ((mode == GL_TRIANGLE_FAN) || (mode == GL_TRIANGLE_STRIP) ||
        (shaded && 
        ((mode == GL_LINES) || (mode == GL_LINE_LOOP) ||
        (mode == GL_LINE_STRIP) || (mode == GL_QUADS)))) {
        
        if (mode == GL_TRIANGLE_STRIP)
            cell_vector (vals, tStrip, sizeof(tStripInit)/sizeof(float));
        else if (mode == GL_TRIANGLE_FAN)
            cell_vector (vals, tFan, sizeof(tFanInit)/sizeof(float));
        else if (mode == GL_QUADS)
            cell_vector (vals, shadeQuad, sizeof(shadeQuadInit)/sizeof(float));
        else
            cell_vector (vals, shadeFour, sizeof(shadeFourInit)/sizeof(float));
        
        if (mode == GL_TRIANGLE_FAN) 
            glBegin (GL_TRIANGLE_FAN);
        else if (mode == GL_TRIANGLE_STRIP)
            glBegin (GL_TRIANGLE_STRIP);
        else if (mode == GL_LINES) 
            glBegin (GL_LINES);
        else if (mode == GL_LINE_LOOP)
            glBegin (GL_LINE_LOOP);
        else if (mode == GL_LINE_STRIP) 
            glBegin (GL_LINE_STRIP);
        else if (mode == GL_QUADS)
            glBegin (GL_QUADS);
        glColor3fv (&vals[0]);
        glVertex2fv (&vals[3]);
        glColor3fv (&vals[5]);
        glVertex2fv (&vals[8]);
        glColor3fv (&vals[10]);
        glVertex2fv (&vals[13]);
        glColor3fv (&vals[15]);
        glVertex2fv (&vals[18]);
        glEnd();
        if (bigVertices) {
            glColor3fv (&vals[0]);
            drawCircle (&vals[3]);
            glColor3fv (&vals[5]);
            drawCircle (&vals[8]);
            glColor3fv (&vals[10]);
            drawCircle (&vals[13]);
            glColor3fv (&vals[15]);
            drawCircle (&vals[18]);
        }
        if (outlined) {
            if (mode == GL_TRIANGLE_FAN) {
                glLineWidth (3.0);
                glBegin (GL_LINE_STRIP);
                glColor3f (1.0f, 1.0f, 1.0f);
                glVertex2fv (&vals[3]);
                glVertex2fv (&vals[8]);
                glVertex2fv (&vals[13]);
                glVertex2fv (&vals[3]);
                glVertex2fv (&vals[18]);
                glVertex2fv (&vals[13]);
                glEnd();
                glLineWidth (1.0);
                glBegin (GL_LINE_STRIP);
                glColor3f (0.0f, 0.0f, 0.0f);
                glVertex2fv (&vals[3]);
                glVertex2fv (&vals[8]);
                glVertex2fv (&vals[13]);
                glVertex2fv (&vals[3]);
                glVertex2fv (&vals[18]);
                glVertex2fv (&vals[13]);
                glEnd();
            }
            else if (mode == GL_TRIANGLE_STRIP)  {
                glLineWidth (3.0);
                glColor3f (1.0f, 1.0f, 1.0f);
                glBegin (GL_LINE_LOOP);
                glVertex2fv (&vals[3]);
                glVertex2fv (&vals[8]);
                glVertex2fv (&vals[18]);
                glVertex2fv (&vals[13]);
                glEnd();
                glBegin (GL_LINES);
                glVertex2fv (&vals[8]);
                glVertex2fv (&vals[13]);
                glEnd();
                glLineWidth (1.0);
                glColor3f (0.0f, 0.0f, 0.0f);
                glBegin (GL_LINE_LOOP);
                glVertex2fv (&vals[3]);
                glVertex2fv (&vals[8]);
                glVertex2fv (&vals[18]);
                glVertex2fv (&vals[13]);
                glEnd();
                glBegin (GL_LINES);
                glVertex2fv (&vals[8]);
                glVertex2fv (&vals[13]);
                glEnd();
            }
        }
    }
    
    else if (!shaded && 
        ((mode == GL_LINE_LOOP) || (mode == GL_LINE_STRIP) || 
        (mode == GL_QUADS))) {
        
        if (mode == GL_QUADS)
            cell_vector (vals, flatQuad, sizeof(flatQuadInit)/sizeof(float));
        else
            cell_vector (vals, flatFour, sizeof(flatFourInit)/sizeof(float));
        if (mode == GL_LINE_LOOP) 
            glBegin (GL_LINE_LOOP);
        else if (mode == GL_LINE_STRIP) 
            glBegin (GL_LINE_STRIP);
        else if (mode == GL_QUADS) 
            glBegin (GL_QUADS);
        glColor3fv (&vals[0]);
        glVertex2fv (&vals[3]);
        glVertex2fv (&vals[5]);
        glVertex2fv (&vals[7]);
        glVertex2fv (&vals[9]);
        glEnd();
        if (bigVertices) {
            glColor3fv (&vals[0]);
            drawCircle (&vals[3]);
            drawCircle (&vals[5]);
            drawCircle (&vals[7]);
            drawCircle (&vals[9]);
        }
    } 
    else if (shaded && 
        ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
        cell_vector (vals, shadeThree, sizeof(shadeThreeInit)/sizeof(float));
        if (mode == GL_TRIANGLES) 
            glBegin (GL_TRIANGLES);
        else if (mode == GL_POLYGON) 
            glBegin (GL_POLYGON);
        glColor3fv (&vals[0]);
        glVertex2fv (&vals[3]);
        glColor3fv (&vals[5]);
        glVertex2fv (&vals[8]);
        glColor3fv (&vals[10]);
        glVertex2fv (&vals[13]);
        glEnd();
        if (bigVertices) {
            glColor3fv (&vals[0]);
            drawCircle (&vals[3]);
            glColor3fv (&vals[5]);
            drawCircle (&vals[8]);
            glColor3fv (&vals[10]);
            drawCircle (&vals[13]);
        }
    }
    else if (!shaded && 
        ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
        cell_vector (vals, flatThree, sizeof(flatThreeInit)/sizeof(float));
        if (mode == GL_TRIANGLES) 
            glBegin (GL_TRIANGLES);
        else if (mode == GL_POLYGON) 
            glBegin (GL_POLYGON);
        glColor3fv (&vals[0]);
        glVertex2fv (&vals[3]);
        glVertex2fv (&vals[5]);
        glVertex2fv (&vals[7]);
        glEnd();
        if (bigVertices) {
            glColor3fv (&vals[0]);
            drawCircle (&vals[3]);
            drawCircle (&vals[5]);
            drawCircle (&vals[7]);
        }
    }
    else if ((mode == GL_POINTS) || (mode == GL_QUAD_STRIP)) {
        if (mode == GL_POINTS) {
            cell_vector (vals, shadePoints, sizeof(shadePointsInit)/sizeof(float));
            glBegin (GL_POINTS);
        }
        else if (mode == GL_QUAD_STRIP) {
            cell_vector (vals, shadeQStrip, sizeof(shadeQStripInit)/sizeof(float));
            glBegin (GL_QUAD_STRIP);
        }
        glColor3fv (&vals[0]);
        glVertex2fv (&vals[3]);
        glColor3fv (&vals[5]);
        glVertex2fv (&vals[8]);
        glColor3fv (&vals[10]);
        glVertex2fv (&vals[13]);
        glColor3fv (&vals[15]);
        glVertex2fv (&vals[18]);
        glColor3fv (&vals[20]);
        glVertex2fv (&vals[23]);
        glColor3fv (&vals[25]);
        glVertex2fv (&vals[28]);
        glEnd();
        if (bigVertices) {
            glColor3fv (&vals[0]);
            drawCircle (&vals[3]);
            glColor3fv (&vals[5]);
            drawCircle (&vals[8]);
            glColor3fv (&vals[10]);
            drawCircle (&vals[13]);
            glColor3fv (&vals[15]);
            drawCircle (&vals[18]);
            glColor3fv (&vals[20]);
            drawCircle (&vals[23]);
            glColor3fv (&vals[25]);
            drawCircle (&vals[28]);
        }
        if (outlined && (mode == GL_QUAD_STRIP)) {
            glLineWidth (3.0);
            glColor3f (1.0f, 1.0f, 1.0f);
            glBegin (GL_LINE_LOOP);
            glVertex2fv (&vals[3]);
            glVertex2fv (&vals[8]);
            glVertex2fv (&vals[18]);
            glVertex2fv (&vals[28]);
            glVertex2fv (&vals[23]);
            glVertex2fv (&vals[13]);
            glEnd();
            glBegin (GL_LINES);
            glVertex2fv (&vals[13]);
            glVertex2fv (&vals[18]);
            glEnd();
            glLineWidth (1.0);
            glColor3f (0.0f, 0.0f, 0.0f);
            glBegin (GL_LINE_LOOP);
            glVertex2fv (&vals[3]);
            glVertex2fv (&vals[8]);
            glVertex2fv (&vals[18]);
            glVertex2fv (&vals[28]);
            glVertex2fv (&vals[23]);
            glVertex2fv (&vals[13]);
            glEnd();
            glBegin (GL_LINES);
            glVertex2fv (&vals[13]);
            glVertex2fv (&vals[18]);
            glEnd();
        }
    }
}

void
drawaxes(void)
{
    glColor3ub(255, 255, 255);
    glLineWidth (2.0);
    glBegin (GL_LINE_STRIP);
    glVertex2i (200, 0);
    glVertex2i (0, 0);
    glVertex2i (0, 200);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex2i (-5, 195);
    glVertex2i (0, 200);
    glVertex2i (5, 195);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex2i (195, -5);
    glVertex2i (200, 0);
    glVertex2i (195, 5);
    glEnd ();
    glLineWidth (1.0);
    
    setfont("helvetica", 12);
    drawstr(205, -3, "X");
    drawstr(-5, 205, "Y");
    drawstr(-10, -10, "0");
    drawstr(-27, 195, "200");
    drawstr(186, -16, "200");
}

void
main_reshape(int width,  int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
#define GAP  25   /* gap between subwindows */
    sub_width = (width-GAP*3)/2.0;
    sub_height = (height-GAP*2);
    
    glutSetWindow(screen);
    glutPositionWindow(GAP, GAP);
    glutReshapeWindow(sub_width, sub_height);
    glutSetWindow(command);
    glutPositionWindow(GAP+sub_width+GAP, GAP);
    glutReshapeWindow(sub_width, sub_height);
}

void
main_display(void)
{
    glClearColor(0.8f, 0.8f, 0.8f, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(0, 0, 0);
    setfont("helvetica", 12);
    drawstr(GAP, GAP-5, "Screen-space view");
    drawstr(GAP+sub_width+GAP, GAP-5, "Command manipulation window");
    glutSwapBuffers();
}

void
main_keyboard(unsigned char key, int x, int y)
{
    int i;
    
    switch (key) {
    case 'B':
        bigVertices = !bigVertices;
        break;
    case 'C':
        shaded = !shaded;
        break;
    case 'O':
        outlined = !outlined;
        break;
    case 'f':
        mode = GL_TRIANGLE_FAN;
        glutSetWindow(screen);
        glutDestroyMenu (sm);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle drawing outlines", 'O');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'g':
        mode = GL_POLYGON;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle colors at vertices", 'C');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'l':
        mode = GL_LINES;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle colors at vertices", 'C');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'n':
        mode = GL_LINE_LOOP;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle colors at vertices", 'C');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'p':
        mode = GL_POINTS;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'q':
        mode = GL_QUADS;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle colors at vertices", 'C');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'r':
        if (!shaded && (mode == GL_LINES)) {
            for (i = 0; i < (sizeof(linesInit)/sizeof(float)); i++)
                lines[i].value = linesInit[i];
        }
        else if (shaded && 
            ((mode == GL_LINES) || (mode == GL_LINE_LOOP) ||
            (mode == GL_LINE_STRIP))) {
            for (i = 0; i < (sizeof(shadeFourInit)/sizeof(float)); i++)
                shadeFour[i].value = shadeFourInit[i];
        }
        else if (!shaded && 
            ((mode == GL_LINE_LOOP) || (mode == GL_LINE_STRIP))) {
            for (i = 0; i < (sizeof(flatFourInit)/sizeof(float)); i++)
                flatFour[i].value = flatFourInit[i];
        }
        else if (shaded && 
            ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
            for (i = 0; i < (sizeof(shadeThreeInit)/sizeof(float)); i++)
                shadeThree[i].value = shadeThreeInit[i];
        }
        else if (!shaded && 
            ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
            for (i = 0; i < (sizeof(flatThreeInit)/sizeof(float)); i++)
                flatThree[i].value = flatThreeInit[i];
        }
        else if (mode == GL_TRIANGLE_FAN) {
            for (i = 0; i < (sizeof(tFanInit)/sizeof(float)); i++)
                tFan[i].value = tFanInit[i];
        }
        else if (mode == GL_TRIANGLE_STRIP) {
            for (i = 0; i < (sizeof(tStripInit)/sizeof(float)); i++)
                tStrip[i].value = tStripInit[i];
        }
        else if (!shaded && (mode == GL_QUADS)) {
            for (i = 0; i < (sizeof(flatQuadInit)/sizeof(float)); i++)
                flatQuad[i].value = flatQuadInit[i];
        }
        else if (shaded && (mode == GL_QUADS)) {
            for (i = 0; i < (sizeof(shadeQuadInit)/sizeof(float)); i++)
                shadeQuad[i].value = shadeQuadInit[i];
        }
        else if (mode == GL_QUAD_STRIP) {
            for (i = 0; i < (sizeof(shadeQStripInit)/sizeof(float)); i++) 
                shadeQStrip[i].value = shadeQStripInit[i];
        }
        else if (mode == GL_POINTS) {
            for (i = 0; i < (sizeof(shadePointsInit)/sizeof(float)); i++) 
                shadePoints[i].value = shadePointsInit[i];
        }
        break;
    case 't':
        mode = GL_TRIANGLES;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle colors at vertices", 'C');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'u':
        mode = GL_QUAD_STRIP;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle drawing outlines", 'O');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'x':
        mode = GL_TRIANGLE_STRIP;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle drawing outlines", 'O');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 'y':
        mode = GL_LINE_STRIP;
        glutSetWindow(screen);
        sm = glutCreateMenu(screen_menu);
        glutAddMenuEntry("Options", 0);
        glutAddMenuEntry("", 0);
        glutAddMenuEntry("Toggle big vertices", 'B');
        glutAddMenuEntry("Toggle colors at vertices", 'C');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 27:
        exit(0);
    default:
        break;
    }
    
    redisplay_all();
}

void
screen_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D ((GLdouble) 100-width/2, (GLdouble) 100+width/2, 
        (GLdouble) 100-height/2, (GLdouble) 100+height/2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void
screen_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawaxes();
    drawmodel();
    glutSwapBuffers();
}

void
command_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void 
command_draw_color(GLuint ypos) {
    drawstr(20, ypos, "glColor3f   (");
    drawstr(170, ypos, ",");
    drawstr(230, ypos, ","); 
    drawstr(290, ypos, ");");
}

void 
command_draw_vertex(GLuint ypos) {
    drawstr(20, ypos, "glVertex2f (");
    drawstr(170, ypos, ",");
    drawstr(230, ypos, ");"); 
}

void 
command_display(void)
{
    int i;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3ub(255, 255, 255);
    setfont("helvetica", 18);
    
    drawstr(20, 30, "glBegin (");
    
    if (mode == GL_LINES)
        drawstr(95, 30, "GL_LINES);");
    else if (mode == GL_LINE_LOOP)
        drawstr(95, 30, "GL_LINE_LOOP);");
    else if (mode == GL_LINE_STRIP)
        drawstr(95, 30, "GL_LINE_STRIP);");
    else if (mode == GL_TRIANGLES)
        drawstr(95, 30, "GL_TRIANGLES);");
    else if (mode == GL_TRIANGLE_FAN)
        drawstr(95, 30, "GL_TRIANGLE_FAN);");
    else if (mode == GL_TRIANGLE_STRIP)
        drawstr(95, 30, "GL_TRIANGLE_STRIP);");
    else if (mode == GL_QUADS)
        drawstr(95, 30, "GL_QUADS);");
    else if (mode == GL_QUAD_STRIP)
        drawstr(95, 30, "GL_QUAD_STRIP);");
    else if (mode == GL_POLYGON)
        drawstr(95, 30, "GL_POLYGON);");
    else if (mode == GL_POINTS)
        drawstr(95, 30, "GL_POINTS);");
    
    if ((mode == GL_LINES) && !shaded ) {
        command_draw_color(lines[0].y);
        command_draw_vertex(lines[3].y);
        command_draw_vertex(lines[5].y);
        command_draw_color(lines[7].y);
        command_draw_vertex(lines[10].y);
        command_draw_vertex(lines[12].y);
        drawstr(20, lines[12].y + 30, "glEnd();");
        
        for (i = 0; i < (sizeof(linesInit)/sizeof(float)); i++) {
            cell_draw(&lines[i]);
        }
    }
    else if ((mode == GL_TRIANGLE_FAN) || (mode == GL_TRIANGLE_STRIP) || 
        (shaded && 
        ((mode == GL_LINES) || (mode == GL_LINE_LOOP) ||
        (mode == GL_LINE_STRIP) || (mode == GL_QUADS)))) {
        
        command_draw_color(shadeFour[0].y);
        command_draw_vertex(shadeFour[3].y);
        command_draw_color(shadeFour[5].y);
        command_draw_vertex(shadeFour[8].y);
        command_draw_color(shadeFour[10].y);
        command_draw_vertex(shadeFour[13].y);
        command_draw_color(shadeFour[15].y);
        command_draw_vertex(shadeFour[18].y);
        drawstr(20, shadeFour[18].y + 30, "glEnd();");
        
        if (mode == GL_TRIANGLE_STRIP) {
            for (i = 0; i < (sizeof(tStripInit)/sizeof(float)); i++) 
                cell_draw(&tStrip[i]);
        }
        else if (mode == GL_TRIANGLE_FAN) {
            for (i = 0; i < (sizeof(tFanInit)/sizeof(float)); i++) 
                cell_draw(&tFan[i]);
        }
        else if (mode == GL_QUADS) {
            for (i = 0; i < (sizeof(shadeQuadInit)/sizeof(float)); i++) 
                cell_draw(&shadeQuad[i]);
        }
        else {
            for (i = 0; i < (sizeof(shadeFourInit)/sizeof(float)); i++) 
                cell_draw(&shadeFour[i]);
        }
    } 
    else if (!shaded && 
        ((mode == GL_LINE_LOOP) || (mode == GL_LINE_STRIP) || 
        (mode == GL_QUADS))) {
        command_draw_color(flatFour[0].y);
        command_draw_vertex(flatFour[3].y);
        command_draw_vertex(flatFour[5].y);
        command_draw_vertex(flatFour[7].y);
        command_draw_vertex(flatFour[9].y);
        drawstr(20, flatFour[9].y + 30, "glEnd();");
        
        if (mode == GL_QUADS) {
            for (i = 0; i < (sizeof(flatQuadInit)/sizeof(float)); i++) 
                cell_draw(&flatQuad[i]);
        }
        else {
            for (i = 0; i < (sizeof(flatFourInit)/sizeof(float)); i++) 
                cell_draw(&flatFour[i]);
        }
    } 
    else if (shaded && 
        ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
        command_draw_color(shadeThree[0].y);
        command_draw_vertex(shadeThree[3].y);
        command_draw_color(shadeThree[5].y);
        command_draw_vertex(shadeThree[8].y);
        command_draw_color(shadeThree[10].y);
        command_draw_vertex(shadeThree[13].y);
        drawstr(20, shadeThree[13].y + 30, "glEnd();");
        
        for (i = 0; i < (sizeof(shadeThreeInit)/sizeof(float)); i++) {
            cell_draw(&shadeThree[i]);
        }
    } 
    else if (!shaded && 
        ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
        command_draw_color(flatThree[0].y);
        command_draw_vertex(flatThree[3].y);
        command_draw_vertex(flatThree[5].y);
        command_draw_vertex(flatThree[7].y);
        drawstr(20, flatThree[7].y + 30, "glEnd();");
        
        for (i = 0; i < (sizeof(flatThreeInit)/sizeof(float)); i++) {
            cell_draw(&flatThree[i]);
        }
    } 
    else if (mode == GL_QUAD_STRIP) {
        command_draw_color(shadeQStrip[0].y);
        command_draw_vertex(shadeQStrip[3].y);
        command_draw_color(shadeQStrip[5].y);
        command_draw_vertex(shadeQStrip[8].y);
        command_draw_color(shadeQStrip[10].y);
        command_draw_vertex(shadeQStrip[13].y);
        command_draw_color(shadeQStrip[15].y);
        command_draw_vertex(shadeQStrip[18].y);
        command_draw_color(shadeQStrip[20].y);
        command_draw_vertex(shadeQStrip[23].y);
        command_draw_color(shadeQStrip[25].y);
        command_draw_vertex(shadeQStrip[28].y);
        drawstr(20, shadeQStrip[28].y + 30, "glEnd();");
        
        for (i = 0; i < (sizeof(shadeQStripInit)/sizeof(float)); i++) {
            cell_draw(&shadeQStrip[i]);
        }
    } 
    else if ((mode == GL_POINTS)) {
        command_draw_color(shadePoints[0].y);
        command_draw_vertex(shadePoints[3].y);
        command_draw_color(shadePoints[5].y);
        command_draw_vertex(shadePoints[8].y);
        command_draw_color(shadePoints[10].y);
        command_draw_vertex(shadePoints[13].y);
        command_draw_color(shadePoints[15].y);
        command_draw_vertex(shadePoints[18].y);
        command_draw_color(shadePoints[20].y);
        command_draw_vertex(shadePoints[23].y);
        command_draw_color(shadePoints[25].y);
        command_draw_vertex(shadePoints[28].y);
        drawstr(20, shadePoints[28].y + 30, "glEnd();");
        
        for (i = 0; i < (sizeof(shadePointsInit)/sizeof(float)); i++) {
            cell_draw(&shadePoints[i]);
        }
    } 
    
    if (!selection) {
        glColor3ub(255, 255, 0);
        drawstr(10, MESSAGEY, "Click on the arguments and");
        drawstr(10, MESSAGEY+30, "move the mouse to modify values.");
    }   
    
    glutSwapBuffers();
}

int old_y;

void
command_mouse(int button, int state, int x, int y)
{
    int i;
    selection = 0;
    
    if (state == GLUT_DOWN) {
        /* mouse should only hit _one_ of the cells, so adding up all
        the hits just propagates a single hit. */
        
        if ((mode == GL_LINES) && !shaded ) {
            for (i = 0; i < (sizeof(linesInit)/sizeof(float)); i++)
                selection += cell_hit(&lines[i], x, y);
        }
        else if ((shaded && 
            ((mode == GL_LINES) || (mode == GL_LINE_LOOP) ||
            (mode == GL_LINE_STRIP)))) {
            
            for (i = 0; i < (sizeof(shadeFourInit)/sizeof(float)); i++)
                selection += cell_hit(&shadeFour[i], x, y);
        }
        else if (!shaded && 
            ((mode == GL_LINE_LOOP) || (mode == GL_LINE_STRIP))) {
            for (i = 0; i < (sizeof(flatFourInit)/sizeof(float)); i++)
                selection += cell_hit(&flatFour[i], x, y);
        }
        else if (shaded && (mode == GL_QUADS)) {
            for (i = 0; i < (sizeof(shadeQuadInit)/sizeof(float)); i++)
                selection += cell_hit(&shadeQuad[i], x, y);
        }
        else if (!shaded && (mode == GL_QUADS)) {
            for (i = 0; i < (sizeof(flatQuadInit)/sizeof(float)); i++)
                selection += cell_hit(&flatQuad[i], x, y);
        }
        else if (mode == GL_TRIANGLE_FAN) {
            for (i = 0; i < (sizeof(tFanInit)/sizeof(float)); i++)
                selection += cell_hit(&tFan[i], x, y);
        }
        else if (mode == GL_TRIANGLE_STRIP) {
            for (i = 0; i < (sizeof(tStripInit)/sizeof(float)); i++)
                selection += cell_hit(&tStrip[i], x, y);
        }
        else if (shaded && 
            ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
            for (i = 0; i < (sizeof(shadeThreeInit)/sizeof(float)); i++)
                selection += cell_hit(&shadeThree[i], x, y);
        }
        else if (!shaded && 
            ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
            for (i = 0; i < (sizeof(flatThreeInit)/sizeof(float)); i++)
                selection += cell_hit(&flatThree[i], x, y);
        }
        else if (mode == GL_QUAD_STRIP) {
            for (i = 0; i < (sizeof(shadeQStripInit)/sizeof(float)); i++)
                selection += cell_hit(&shadeQStrip[i], x, y);
        }
        else if (mode == GL_POINTS) {
            for (i = 0; i < (sizeof(shadePointsInit)/sizeof(float)); i++)
                selection += cell_hit(&shadePoints[i], x, y);
        }
    }
    
    old_y = y;
    
    redisplay_all();
}

void
command_motion(int x, int y)
{
    int i;
    
    if ((mode == GL_LINES) && !shaded ) {
        for (i = 0; i < (sizeof(linesInit)/sizeof(float)); i++)
            cell_update(&lines[i], old_y-y);
    }
    else if (shaded && 
        ((mode == GL_LINES) || (mode == GL_LINE_LOOP) ||
        (mode == GL_LINE_STRIP))) {
        for (i = 0; i < (sizeof(shadeFourInit)/sizeof(float)); i++)
            cell_update(&shadeFour[i], old_y-y);
    }
    else if (mode == GL_TRIANGLE_STRIP) {
        for (i = 0; i < (sizeof(tStripInit)/sizeof(float)); i++)
            cell_update(&tStrip[i], old_y-y);
    }
    else if (mode == GL_TRIANGLE_FAN) {
        for (i = 0; i < (sizeof(tFanInit)/sizeof(float)); i++)
            cell_update(&tFan[i], old_y-y);
    }
    else if (shaded && (mode == GL_QUADS)) {
        for (i = 0; i < (sizeof(shadeQuadInit)/sizeof(float)); i++)
            cell_update(&shadeQuad[i], old_y-y);
    }
    else if (!shaded && (mode == GL_QUADS)) {
        for (i = 0; i < (sizeof(flatFourInit)/sizeof(float)); i++)
            cell_update(&flatQuad[i], old_y-y);
    }
    else if (!shaded && 
        ((mode == GL_LINE_LOOP) || (mode == GL_LINE_STRIP))) {
        for (i = 0; i < (sizeof(flatFourInit)/sizeof(float)); i++)
            cell_update(&flatFour[i], old_y-y);
    }
    else if (shaded && 
        ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
        for (i = 0; i < (sizeof(shadeThreeInit)/sizeof(float)); i++)
            cell_update(&shadeThree[i], old_y-y);
    }
    else if (!shaded && 
        ((mode == GL_TRIANGLES) || (mode == GL_POLYGON))) {
        for (i = 0; i < (sizeof(flatThreeInit)/sizeof(float)); i++)
            cell_update(&flatThree[i], old_y-y);
    }
    else if (mode == GL_QUAD_STRIP) {
        for (i = 0; i < (sizeof(shadeQStripInit)/sizeof(float)); i++)
            cell_update(&shadeQStrip[i], old_y-y);
    }
    else if (mode == GL_POINTS) {
        for (i = 0; i < (sizeof(shadePointsInit)/sizeof(float)); i++)
            cell_update(&shadePoints[i], old_y-y);
    }
    old_y = y;
    
    redisplay_all();
}

void
screen_menu(int value)
{
    main_keyboard((unsigned char)value, 0, 0);
}


void
command_menu(int value)
{
    main_keyboard((unsigned char)value, 0, 0);
}

void
redisplay_all(void)
{
    glutSetWindow(command);
    glutPostRedisplay();
    glutSetWindow(screen);
    screen_reshape(sub_width, sub_height);
    glutPostRedisplay();
}

int
main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(640+GAP*3, 512+GAP*2);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);
    
    window = glutCreateWindow("Shapes");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_keyboard);
    
    screen = glutCreateSubWindow(window, GAP, GAP, 256, 256);
    glutReshapeFunc(screen_reshape);
    glutDisplayFunc(screen_display);
    glutKeyboardFunc(main_keyboard);
    sm = glutCreateMenu(screen_menu);
    glutAddMenuEntry("Options", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Toggle big vertices", 'B');
    glutAddMenuEntry("Specify colors@vertices", 'C');
    glutAddMenuEntry("Toggle drawing outlines", 'O');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    createCircleList ();
    
    command = glutCreateSubWindow(window, GAP+256+GAP, GAP, 256, 256);
    glutReshapeFunc(command_reshape);
    glutDisplayFunc(command_display);
    glutMotionFunc(command_motion);
    glutMouseFunc(command_mouse);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(command_menu);
    glutAddMenuEntry("Shapes", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("POINTS [p]  ", 'p');
    glutAddMenuEntry("LINES [l]  ", 'l');
    glutAddMenuEntry("LINE_LOOP [n]", 'n');
    glutAddMenuEntry("LINE_STRIP [y]", 'y');
    glutAddMenuEntry("TRIANGLES [t]", 't');
    glutAddMenuEntry("TRIANGLE_FAN [f]", 'f');
    glutAddMenuEntry("TRIANGLE_STRIP [x]", 'x');
    glutAddMenuEntry("QUADS [q]", 'q');
    glutAddMenuEntry("QUAD_STRIP [u]", 'u');
    glutAddMenuEntry("POLYGON [g]", 'g');
    glutAddMenuEntry("Reset parameters [r]", 'r');
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Quit", 27);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    
    redisplay_all();
    
    glutMainLoop();
    
    return 0;
}
