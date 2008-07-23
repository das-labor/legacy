/* logs.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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
