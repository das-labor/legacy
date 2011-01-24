#ifndef _H_DC_COMMANDS
#define _H_DC_COMMANDS

#define PIXEL_OFF 0
#define PIXEL_ON 1

#define DC_RESET                           0x00
#define DC_DRAW_CROSS                      0x01 //(uint16_t x, uint16_t y)        draw a cross.
#define DC_SET_COLOR                       0x02 //(uint8_t color)                 sets color in which things are drawn
#define DC_CLEAR_SCREEN                    0x03 //(void)
#define DC_DRAW_RECTANGLE                  0x04 //(rectangle_t)
#define DC_FILL_RECTANGLE                  0x05 //(rectangle_t)
#define DC_DRAW_STRING_IN_RECT             0x06 //(rectangle_t + string)
#define DC_DRAW_STRING_IN_RECT_VERT        0x07 //(rectangle_t + string)
#define DC_DRAW_ICON                       0x08 //(x,y,size,icon_t)
#define DC_DRAW_PIXEL                      0x09 //(x,y)
#define DC_DRAW_STRING                     0x0A //(x,y,string)

#endif // _H_DC_COMMANDS

