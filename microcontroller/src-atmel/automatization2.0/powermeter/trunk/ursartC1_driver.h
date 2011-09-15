#include "avr_compiler.h"
#include "config.h"

void sendUSARTC1_putstr(char *str);
void sendUSARTC1_putstr_P(PGM_P str);
void sendUSARTC1_char(char c);
void InitializeUSARTC1();