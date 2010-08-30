#include <avr/boot.h>

extern unsigned char Station_id;

void bootloader(void) BOOTLOADER_SECTION __attribute__ ((naked)); 
