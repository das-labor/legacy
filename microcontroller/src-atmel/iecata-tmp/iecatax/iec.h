
#include <avrx.h>

extern volatile uint8_t error;

/* error numbers, given in bcd */
#define NO_ERROR         0x00
#define INIT_ERROR       0x74
#define CREATE_ERROR     0x01
#define NOT_OPEN_ERROR   0x61
#define NOT_FOUND_ERROR  0x62
#define SYNTAX_ERROR     0x30
#define VERSION_ERROR    0x73

AVRX_EXTERNTASK(iec_task);
AVRX_EXTERNTASK(led_task);

uint8_t iecListen(uint8_t *buf, uint16_t *index, uint16_t size);
