
//#include <avr/boot.h>
#include "sp_driver.h"

//#include "pgmspace.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>


#include "../config.h"
#include "bootloader.h"

#include "can.h"
#include "util.h"

#include "flash_a3revb_workaround.h"

unsigned char Station_id;

typedef struct
{
	unsigned char cmd;
	unsigned int index;
	unsigned int size;
	unsigned int address;
} sdo_message;

typedef struct
{
	unsigned int data[4];
} sdo_data_message;


//unsigned char Device_info_msg[] __attribute__ ((section (".progdata"))) =
const unsigned char Device_info_msg[] PROGMEM =
{
	SDO_CMD_REPLY,
	SDO_TYPE_UINT32_RO,
	(unsigned char)(SPM_PAGESIZE),
	(unsigned char)(SPM_PAGESIZE >> 8),
	(FLASHEND + 1ul) / 1024ul, //changed this from Atmega number to real Flash-size in kB
	0
};

//unsigned char Flash_info_msg[] __attribute__ ((section (".progdata"))) =
const unsigned char Flash_info_msg[] PROGMEM =
{
	SDO_CMD_REPLY,
	SDO_TYPE_STRING_WO,
#if (FLASHEND >= 0xffff)
	0xff,0xff //dirty hack : return 65535 bytes instead of 65536 because we used to small sized integer...
#else
	(unsigned char)((unsigned char)FLASHEND + 1),
	((unsigned int)FLASHEND + 1) >> 8
#endif
};


uint8_t data_buffer[SPM_PAGESIZE];


void sync_osc() {
	/*32 MHz Oszillator starten und 32.768kHz Oszillator für DFLL starten*/
	OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm ;
	
	/*Wenn Oscillator stabil wird das Flag RC32MRDY
	 * gesetzt und 32Mhz können benutzt werden*/
	while (!(OSC.STATUS & OSC_RC32MRDY_bm));


	/* auto kalibierung ein */
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	/*Clock auf 32Mhz einstellen*/
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

}

uint8_t can_address EEMEM = 5;

int main(void) {
	uint16_t Address;
	uint16_t Size;
	
	uint16_t buffer_address;
	unsigned char x;
	
	sync_osc();
	init_workaround();
	
	Station_id = eeprom_read_byte(0);
	
	can_init();

	Tx_msg.addr_src = Station_id;
	Tx_msg.addr_dst = 0;
	Tx_msg.port_src = PORT_MGT;
	Tx_msg.port_dst = PORT_MGT;
	Tx_msg.dlc = 1;
	Tx_msg.data[0] = FKT_MGT_AWAKE;
	
	can_transmit();

	#if defined(TOGGLE_MCP_LED)
		unsigned char toggle = 0x1C;
	#elif defined(TOGGLE_PORT_LED)
		PORT_LED.DIRSET = (1<<BIT_LED);
	#endif
	
	unsigned char count = 20;
	while (count--) {
		#if defined(TOGGLE_MCP_LED)
			mcp_write(BFPCTRL, toggle);
			toggle ^= 0x10;
		#elif defined(TOGGLE_PORT_LED)
			PORT_LED.OUTTGL = (1<<BIT_LED);
		#endif
		_delay_ms(100);
		
		if (can_get_nb()) {
			goto sdo_server;
		}
	}
	
	start_app:
	
	asm volatile(JUMP_OPCODE " 0\r\t");
	
	sdo_server:
	
	while (1) {
		if (Rx_msg.port_dst == PORT_SDO_CMD) {
			sdo_message * msg = (sdo_message*)Rx_msg.data;
			
			Tx_msg.port_src = PORT_SDO_CMD;
			Tx_msg.addr_dst = Rx_msg.addr_src;
			Tx_msg.port_dst = Rx_msg.port_src;
			
			
			if (msg->cmd == SDO_CMD_READ) {
				switch (msg->index) {
					case 0xFF00:	//device information
						my_memcpy_P(Tx_msg.data, Device_info_msg, sizeof(Device_info_msg));
						Tx_msg.dlc = sizeof(Device_info_msg);
						break;
					case 0xFF01:	//upload flash
						my_memcpy_P(Tx_msg.data, Flash_info_msg, sizeof(Flash_info_msg));
						Tx_msg.dlc = sizeof(Flash_info_msg);
						break;
					case 0xFF02:
						goto start_app;
					default:
						Tx_msg.dlc = 1;
						Tx_msg.data[0] = SDO_CMD_ERROR_INDEX;
						break;
				}
				can_transmit();
			} else if (msg->cmd == SDO_CMD_WRITE_BLK) {
				if (msg->index == 0xFF01) {
					Address = msg->address;
					Size = msg->size;
					Tx_msg.dlc = 1;
					Tx_msg.data[0] = SDO_CMD_WRITE_BLK_ACK;
					can_transmit();
					goto programm;
				} else {
					Tx_msg.dlc = 1;
					Tx_msg.data[0] = SDO_CMD_ERROR_INDEX;
					can_transmit();
				}
			}
		}
		while (!can_get_nb());
	}

	programm:
	ClearFlashBuffer();
	
	while (1) {
		PORT_LED.OUTTGL = _BV(BIT_LED);
		while (!can_get_nb());
		if (Rx_msg.port_dst != PORT_SDO_DATA) {
			//boot_rww_enable ();
			goto sdo_server;
		} else {
			for (x = 0; x < 4; x++) {
				LoadFlashWord(Address, ((sdo_data_message*)Rx_msg.data)->data[x]);
				Address += 2;
			}

			if ((Address % SPM_PAGESIZE) == 0) {
				EraseWriteApplicationPage(Address - SPM_PAGESIZE);
			}

			can_transmit();		//this will repeat the ACK message
		}
	}
}

