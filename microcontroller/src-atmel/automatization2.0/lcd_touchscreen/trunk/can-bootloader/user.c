
#include <avr/io.h>
#include "canboot_lib/bootloader.h"

/*


#define ATN_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ATN);DDR_HANDSHAKE |= _BV(BIT_ATN);}
#define ATN_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ATN);PORT_HANDSHAKE |= _BV(BIT_ATN);}

#define ACK_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ACK);DDR_HANDSHAKE |= _BV(BIT_ACK);}
#define ACK_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ACK);PORT_HANDSHAKE |= _BV(BIT_ACK);}

#define WAIT_ACK_LOW()     {while (PIN_HANDSHAKE & _BV(BIT_ACK));}
#define WAIT_ACK_HIGH()    {while ((PIN_HANDSHAKE & _BV(BIT_ACK)) == 0);}

#define WAIT_ATN_LOW()     {while (PIN_HANDSHAKE & _BV(BIT_ATN));}
#define WAIT_ATN_HIGH()    {while ((PIN_HANDSHAKE & _BV(BIT_ATN)) == 0);}

#define ATN_IS_LOW()   ((PIN_HANDSHAKE & _BV(BIT_ATN))==0)
#define ACK_IS_LOW()   ((PIN_HANDSHAKE & _BV(BIT_ACK))==0)


void dc_byte_put(uint8_t b) {
	PORT_DC_DATA = b;

	WAIT_ATN_HIGH();
	ATN_PULL();
	WAIT_ACK_LOW();
	ATN_RELEASE();
	WAIT_ACK_HIGH();
	WAIT_ATN_HIGH();
}

void init_dc_com() {
	DDR_HANDSHAKE  &= ~(_BV(BIT_ATN) | _BV(BIT_ACK));
	PORT_HANDSHAKE |=  (_BV(BIT_ATN) | _BV(BIT_ACK));
	DDR_DC_DATA = 0xff;
	int x;
	for(x = 0; x < 32; x++) {
		dc_byte_put(DC_RESET);
	}
}
*/

void user_init(){

}


void user_bootloader_entry(void){

}

