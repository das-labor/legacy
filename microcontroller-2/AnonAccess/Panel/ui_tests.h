#ifndef UI_TESTS_H_
#define UI_TESTS_H_


#include <stdint.h>
#include "lcd_tools.h"
#include "ui_primitives.h"

extern menu_t ui_test_menu_mt[];

void read_flash(void);
void read_decimal(void);
void read_hex(void);
void read_str(void);
void display_analysis(void);

#endif /*UI_TESTS_H_*/
