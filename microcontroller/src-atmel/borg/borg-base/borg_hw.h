#ifndef BORG_HW_H
#define BORG_HW_H

#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


void borg_hw_init();

#endif
