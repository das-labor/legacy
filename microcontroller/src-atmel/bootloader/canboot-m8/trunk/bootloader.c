#include <avr/boot.h>
//#include "pgmspace.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "config.h"
#include "bootloader.h"

#include "can.h"
#include "util.h"

#define PORT_MGT 0x30
#define FKT_MGT_AWAKE 0x03

unsigned char Station_id __attribute__ ((section (".eeprom"))) = CAN_ADDR;

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


unsigned char Device_info_msg[] __attribute__ ((section (".progdata"))) ={
	SDO_CMD_REPLY,
	SDO_TYPE_UINT32_RO,
	(unsigned char)SPM_PAGESIZE,
	(unsigned char)SPM_PAGESIZE>>8,
	32,
	0
};

unsigned char Flash_info_msg[] __attribute__ ((section (".progdata"))) ={
	SDO_CMD_REPLY,
	SDO_TYPE_STRING_WO,
	(unsigned char)((unsigned char)FLASHEND+1),
	((unsigned int)FLASHEND+1)>>8
};


static inline void delay_100ms(){
	unsigned int x;
	for(x=0;x<65535;x++){
		asm volatile("nop");
	}
}

unsigned char Station_id;


int bootloader(void){
	uint16_t Address;
	uint16_t Size;
	unsigned char x;
	
	cli();
	
	asm volatile(
		"out 0x3e, %A0\n\t"
		"out 0x3d, %B0\n\t"
		::"w" (RAMEND)
	);
	
	EEAR = EEPR_ADDR_NODE;
	EECR = (1<<EERE);
	Station_id = EEDR;
		
	can_init();
	
	Tx_msg.addr_src = EEDR;
	Tx_msg.addr_dst = 0;
	Tx_msg.port_src = PORT_MGT;
	Tx_msg.port_dst = PORT_MGT;
	Tx_msg.dlc = 1;
	Tx_msg.data[0] = FKT_MGT_AWAKE;
	
	can_transmit();
	
	unsigned char count=20, toggle=0x1C;
	while(count--){
		mcp_write(BFPCTRL, toggle);
		toggle ^= 0x10;
		delay_100ms();
		
		if(can_get_nb()){
			goto sdo_server;
		}
	}
	
	start_app:
	asm volatile("rjmp __vectors");
	
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
						my_memcpy_P(sizeof(Device_info_msg), Tx_msg.data, Device_info_msg);
						Tx_msg.dlc = sizeof(Device_info_msg);
						break;
					case 0xFF01:	//upload flash
						my_memcpy_P(sizeof(Flash_info_msg), Tx_msg.data, Flash_info_msg);
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
