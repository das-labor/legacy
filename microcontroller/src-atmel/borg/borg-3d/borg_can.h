
extern unsigned char borg_mode;

void bcan_init();
unsigned char bcan_mode();
void bcan_process_messages();
void watchdog_enable();
void timer0_off();
