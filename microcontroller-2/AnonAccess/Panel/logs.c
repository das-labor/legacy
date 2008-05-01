
#include "ui_primitives.h"
#include "logs.h"

ui_loglist_t bootlog, syslog, seclog;

void init_logs(void){
	ui_loginit(&bootlog, BOOTLOG_SIZE);
	ui_loginit(&syslog,  SYSLOG_SIZE);
	ui_loginit(&seclog,  SECLOG_SIZE);
}


