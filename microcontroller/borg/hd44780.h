/* Header for hd44780 library by Peter Fuhrmann*/

#include <avr/pgmspace.h>

#define DISP_LEN 16
#define DISP_LINES 1

void hd44780_init();

void hd44780_print(char * string);
void hd44780_print_P(PGM_P string);

void hd44780_data(uint8_t);

void hd44780_load_font_P(uint8_t charnum, PGM_P data);

void hd44780_set_cursor(uint8_t row, uint8_t col);
void hd44780_backspace(uint8_t num);
void hd44780_clear_line(uint8_t line);


#define hd44780_printl(line,string) hd44780_set_cursor(line,0);hd44780_print(string);
