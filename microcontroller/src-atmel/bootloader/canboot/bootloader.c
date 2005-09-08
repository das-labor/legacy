#include <avr/boot.h>
//#include "pgmspace.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "config.h"
#include "bootloader.h"

#include "spi.h"
#include "can.h"

void delay_100ms() BOOTLOADER_SECTION;
void boot_program_page (uint16_t page, uint8_t *buf) BOOTLOADER_SECTION;

unsigned char Station_id;

#define SDO_CMD_READ 		0x20
#define SDO_CMD_REPLY 		0x21
#define SDO_CMD_INFO 		0x22
#define SDO_CMD_READ_BLK	0x40
#define SDO_CMD_READ_BLK_ACK	0x41
#define SDO_CMD_WRITE_BLK	0x48
#define SDO_CMD_WRITE_BLK_ACK	0x49


#define SDO_CMD_ERROR_INDEX 	0x80

#define SDO_TYPE_UINT8_RO 0x00
#define SDO_TYPE_UINT8_RW 0x01
#define SDO_TYPE_UINT16_RO 0x04
#define SDO_TYPE_UINT16_RW 0x05
#define SDO_TYPE_UINT32_RO 0x08
#define SDO_TYPE_UINT32_RW 0x09
#define SDO_TYPE_STRING_RO 0x80
#define SDO_TYPE_STRING_RW 0x81
#define SDO_TYPE_STRING_WO 0x82

typedef struct{
	unsigned char cmd;
	unsigned int index;
	unsigned int size;
	unsigned int address;
}sdo_message;

typedef struct{
	unsigned int data[4];
}sdo_data_message;



unsigned char Device_info_msg[] PROGMEM ={
	SDO_CMD_REPLY,
	SDO_TYPE_UINT32_RO,
	(unsigned char)SPM_PAGESIZE,
	(unsigned char)SPM_PAGESIZE>>8,
	32,
	0
};

unsigned char Flash_info_msg[] PROGMEM ={
	SDO_CMD_REPLY,
	SDO_TYPE_STRING_WO,
	(unsigned char)((unsigned char)FLASHEND+1),
	((unsigned int)FLASHEND+1)>>8
};

int bootloader(void) {
	uint16_t Address;
	uint16_t Size;
	
	unsigned char x;
	
	cli();
	
	Station_id = 0x35;
		
	can_init();
	
	Tx_msg.addr_src = Station_id;
	Tx_msg.addr_dst = 0;
	Tx_msg.port_src = PORT_WAKEUP;
	Tx_msg.port_dst = PORT_WAKEUP;
	Tx_msg.dlc = 0;
	
	can_transmit();
	
	unsigned char count=20;
	while(count--){
		if(can_get_nb()){
			goto sdo_server;
		}
		
		can_setled(0,count&0x01);
		delay_100ms();
	}
	asm volatile("jmp 0");
	
	sdo_server:
	
	while(1){
		if(Rx_msg.port_dst == PORT_SDO_CMD){
			sdo_message * msg = (sdo_message*)Rx_msg.data;
			
			Tx_msg.port_src = PORT_SDO_CMD;
			Tx_msg.addr_dst = Rx_msg.addr_src;
			Tx_msg.port_dst = Rx_msg.port_src;
			
			
			if(msg->cmd == SDO_CMD_READ){
				switch(msg->index){
					case 0xFF00:	//device information
						memcpy_P(Tx_msg.data, Device_info_msg, sizeof(Device_info_msg));
						Tx_msg.dlc = sizeof(Device_info_msg);
						break;
					case 0xFF01:	//upload flash
						memcpy_P(Tx_msg.data, Flash_info_msg, sizeof(Flash_info_msg));
						Tx_msg.dlc = sizeof(Flash_info_msg);
						break;
					default:
						Tx_msg.dlc = 1;
						Tx_msg.data[0] = SDO_CMD_ERROR_INDEX;	
						break;
				}
				can_transmit();
			}else if(msg->cmd == SDO_CMD_WRITE_BLK){
				if(msg->index==0xFF01){
					Address = msg->address;
					Size = msg->size;
					Tx_msg.dlc = 1;
					Tx_msg.data[0] = SDO_CMD_WRITE_BLK_ACK;
					can_transmit();
					goto programm;
				}else{
					Tx_msg.dlc = 1;
					Tx_msg.data[0] = SDO_CMD_ERROR_INDEX;
					can_transmit();
				}
			
			
			}
		}
		
		while(!can_get_nb());
	}
	
	programm:
	
	while(1){
		while(!can_get_nb());
		if(Rx_msg.port_dst != PORT_SDO_DATA){
			boot_rww_enable ();
			goto sdo_server;
		}else{
			for(x=0;x<4;x++){
				boot_page_fill (Address,((sdo_data_message*)Rx_msg.data)->data[x] );
				Address += 2;
			}
			
			if((Address%SPM_PAGESIZE)==0){
				boot_page_erase (Address-SPM_PAGESIZE);
				boot_spm_busy_wait ();      // Wait until the memory is erased.
				boot_page_write (Address-SPM_PAGESIZE);     // Store buffer in flash page.
				boot_spm_busy_wait();       // Wait until the memory is written.
			}
			
			can_transmit();//this will repeat the ACK message
		}
	}
}

/*
void boot_program_page (uint16_t page, uint8_t *buf)
    {
        uint16_t i;

        eeprom_busy_wait ();

        

        for (i=0; i<SPM_PAGESIZE; i+=2)
        {
            // Set up little-endian word.

            uint16_t w = *buf++;
            w += (*buf++) << 8;

            boot_page_fill (page + i, w);
        }
	
	boot_page_erase (page);
        boot_spm_busy_wait ();      // Wait until the memory is erased.
        boot_page_write (page);     // Store buffer in flash page.
        boot_spm_busy_wait();       // Wait until the memory is written.

        // Reenable RWW-section again. We need this if we want to jump back
        // to the application after bootloading.

        boot_rww_enable ();
}
*/

void delay_100ms(){
	unsigned int x;
	for(x=0;x<65535;x++){
		asm volatile("nop");
	}
}

