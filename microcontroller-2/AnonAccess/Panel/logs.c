
#include "ui_primitives.h"
#include "logs.h"

ui_loglist_t bootlog, syslog, seclog, masterlog;

void init_logs(void){
	ui_loginit(&bootlog,   BOOTLOG_SIZE);
	ui_loginit(&syslog,    SYSLOG_SIZE);
	ui_loginit(&seclog,    SECLOG_SIZE);
	ui_loginit(&masterlog, MASTERLOG_SIZE);
}


