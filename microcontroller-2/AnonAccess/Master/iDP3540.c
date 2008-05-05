
#include "config.h"
#include "i2c_printer.h"
#include "iDP3540.h"


void cutpaper(void){
	printer_str_P(PSTR(FULLCUT_CMDS));
	printer_char(0x00);
}

void setred(void){
	printer_char(REDPRINT_CMD);
}


