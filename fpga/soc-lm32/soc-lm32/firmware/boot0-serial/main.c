/**
 * Primitive first stage bootloader 
 *
 *
 */
#include "soc-hw.h"

/* prototypes */
uint32_t read_uint32()
{
	uint32_t val = 0, i;

    for (i = 0; i < 4; i++) {
        val <<= 8;
        val += (uint8_t)uart_getchar();
    }

    return val;
}

int main(int argc, char **argv)
{
	int8_t  *p;
	uint8_t  c;

	// Initialize UART
	uart_init();

	c = '*'; // print msg on first iteration
	for(;;) {
		uint32_t start, size; 

		switch (c) {
    		case 'u': // upload 
    			start = read_uint32();
    			size  = read_uint32();
    			for (p = (int8_t *) start; p < (int8_t *) (start+size); p++) {
    				*p = uart_getchar();
    			}
    			break;
			case 'd': // download
    			start = read_uint32();
    			size  = read_uint32();
    			for (p = (int8_t *) start; p < (int8_t *) (start+size); p++) {
					uart_putchar( *p );
    			}
    			break;
    		case 'g': // goto
    			start = read_uint32();
    			jump(start);
    			break;   
			default:
				uart_putstr("**soc-lm32/bootloader** > \r\n");
				break;
		};

		c = uart_getchar();

	}
}

