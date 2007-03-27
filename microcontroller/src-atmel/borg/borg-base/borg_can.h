#ifndef BORG_CAN_H_
#define BORG_CAN_H_

extern unsigned char borg_mode;
extern char scrolltext_text[];

void bcan_init();
unsigned char bcan_mode();
void bcan_process_messages();

#endif /* BORG_CAN_H */
