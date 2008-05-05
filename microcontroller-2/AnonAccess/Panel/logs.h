#ifndef LOGS_H_
#define LOGS_H_

#include "ui_primitives.h"
#include "config.h"

extern ui_loglist_t bootlog, syslog, seclog, masterlog;

#ifndef BOOTLOG_SIZE
 #define BOOTLOG_SIZE 16
#endif 

#ifndef SYSLOG_SIZE
 #define SYSLOG_SIZE 16
#endif

#ifndef SECLOG_SIZE
 #define SECLOG_SIZE 16
#endif


#ifndef MASTERLOG_SIZE
 #define MASTERLOG_SIZE 16
#endif

#define BOOTLOG_APPEND_P(s)    ui_logappend(&bootlog, PSTR(s), flash_st, processing_st);
#define BOOTLOG_APPEND_OK      ui_logchangelaststate(&bootlog, success_st);
#define BOOTLOG_APPEND_FAILED  ui_logchangelaststate(&bootlog, failed_st);
#define BOOTLOG_APPEND_INFO    ui_logchangelaststate(&bootlog, informative_st);

#define SYSLOG_APPEND_P(s)    ui_logappend(&syslog, PSTR(s), flash_st, processing_st);
#define SYSLOG_APPEND_OK      ui_logchangelaststate(&syslog, success_st);
#define SYSLOG_APPEND_FAILED  ui_logchangelaststate(&syslog, failed_st);
#define SYSLOG_APPEND_INFO    ui_logchangelaststate(&syslog, informative_st);

#define SECLOG_APPEND_P(s)    ui_logappend(&seclog, PSTR(s), flash_st, processing_st);
#define SECLOG_APPEND_OK      ui_logchangelaststate(&seclog, success_st);
#define SECLOG_APPEND_FAILED  ui_logchangelaststate(&seclog, failed_st);
#define SECLOG_APPEND_INFO    ui_logchangelaststate(&seclog, informative_st);

void init_logs(void);

#endif /*LOGS_H_*/
