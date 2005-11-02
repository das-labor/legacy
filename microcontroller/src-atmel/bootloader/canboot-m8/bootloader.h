#include <avr/boot.h>

extern unsigned char Station_id;

int bootloader(void) BOOTLOADER_SECTION __attribute__ ((naked)); 
