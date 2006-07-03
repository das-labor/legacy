#include "can.h"
#include "spi.h"

#include <stdlib.h>
#include <string.h>
#include "simpleio.h"

#define PORT_SDO_CMD 21
#define PORT_SDO_DATA 22



#define PORT_MGT 0x30
#define FKT_MGT_AWAKE 0x03

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
	unsigned char data[8];
}sdo_data_message;

#define SPM_PAGESIZE 1024

unsigned char Device_info_msg[] ={
	SDO_CMD_REPLY,
	SDO_TYPE_UINT32_RO,
	0,//(unsigned char)SPM_PAGESIZE,
	4,//(unsigned char)(SPM_PAGESIZE/256),
	64,
	0
};

#define FLASHEND 51216

unsigned char Flash_info_msg[] ={
	SDO_CMD_REPLY,
	SDO_TYPE_STRING_WO,
	(unsigned char)((unsigned char)FLASHEND+1),
	((unsigned int)FLASHEND+1)>>8
};


void delay_100ms(){
	unsigned int x;
	for(x=0;x<0x4000;x++){
		asm ("nop");
	}
}



unsigned char Station_id=0x64;

/*

void hexdump(unsigned char * addr, unsigned char size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		itoa(*addr++, sbuf, 16);
		if (sbuf[1] == 0) print("0");
		print(sbuf);
		print(" ");
		if(++x == 16){
			print("\n");
			x = 0;
		}
	}
}


void dump(){
	print("Can Dump:\n");
	while(1){
		cm = can_get_nb();
		if(cm){
			print("Received Message:\n");
			hexdump((void*)cm, (cm->dlc)+5);
			print("\n");
		}
	
	}	
}

*/

void boot_page_fill(unsigned int address, unsigned char c){
	static unsigned int addresss = 0x801;
	(*(unsigned char*)addresss) = c;
	addresss++;
}

can_message Tx_msg, *Rx_msg;

int main(){
	can_message * cm;
	unsigned int Address;
	unsigned int Size;
	unsigned char count=20, toggle=1;
	
	
	asm ("sei");
	
	
	print("\nC64 CAN Bootloader by TIXIV\n\n");
	print("SPI Init...");
	if(spi_init()){
		print("timeout\n");
		return 0;		
	}
	
	print("\nCAN Init...\n");
	can_init();

	Tx_msg.addr_src = Station_id;
	Tx_msg.addr_dst = 0;
	Tx_msg.port_src = PORT_MGT;
	Tx_msg.port_dst = PORT_MGT;
	Tx_msg.dlc = 1;
	Tx_msg.data[0] = FKT_MGT_AWAKE;
	
	
	
	while(1){
		can_transmit(&Tx_msg);
		while(count--){
			//blink LED
			can_setled(0, toggle);
			toggle ^= 1;
			delay_100ms();
		
			if(Rx_msg = can_get_nb()){
				goto sdo_server;
			}
		}
	}
	
	sdo_server:
	
	while(1){
		if(Rx_msg->port_dst == PORT_SDO_CMD){
			sdo_message * msg = (sdo_message*)Rx_msg->data;
			
			Tx_msg.port_src = PORT_SDO_CMD;
			Tx_msg.addr_dst = Rx_msg->addr_src;
			Tx_msg.port_dst = Rx_msg->port_src;
			
			
			if(msg->cmd == SDO_CMD_READ){
				switch(msg->index){
					case 0xFF00:	//device information
						memcpy( Tx_msg.data, Device_info_msg, sizeof(Device_info_msg));
						Tx_msg.dlc = sizeof(Device_info_msg);
						break;
					case 0xFF01:	//upload flash
						memcpy( Tx_msg.data, Flash_info_msg, sizeof(Flash_info_msg));
						Tx_msg.dlc = sizeof(Flash_info_msg);
						break;
					case 0xFF02:
						return(0);
					default:
						Tx_msg.dlc = 1;
						Tx_msg.data[0] = SDO_CMD_ERROR_INDEX;
						break;
				}
				can_transmit(&Tx_msg);
			}else if(msg->cmd == SDO_CMD_WRITE_BLK){
				if(msg->index==0xFF01){
					Address = msg->address;
					Size = msg->size;
					Tx_msg.dlc = 1;
					Tx_msg.data[0] = SDO_CMD_WRITE_BLK_ACK;
					can_transmit(&Tx_msg);
					goto programm;
				}else{
					Tx_msg.dlc = 1;
					Tx_msg.data[0] = SDO_CMD_ERROR_INDEX;
					can_transmit(&Tx_msg);
				}
			
			
			}
		}
		while(!can_get_nb());
	}
	
	programm:
	
	while(1){
		while(!can_get_nb());
		if(Rx_msg->port_dst != PORT_SDO_DATA){
			goto sdo_server;
		}else{
			unsigned char x;
			for(x=0;x<8;x++){
				boot_page_fill (Address,((sdo_data_message*)Rx_msg->data)->data[x] );
				Address += 1;
			}
			
			can_transmit(&Tx_msg);//this will repeat the ACK message
		}
	}
	

}
