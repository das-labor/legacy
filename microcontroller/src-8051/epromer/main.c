
#include <8052.h>
#include "uart.h"

#define uint16_t unsigned int
#include "eprom.h"

#define FLAG_OVERPROGRAMM 1


xdata eprom_param_t params= {
	15,//unsigned char addrlines;
	0x50,//unsigned char vpp;
	0x0100,//unsigned int pgm_time;
	10,//unsigned char pgm_cycles;
	0,//unsigned char flags;
	METHOD_eprom_std,//enum pgm_method method;
	{0x87,0xe3},//pin pin_oe;
	{0xff,0xff},//pin pin_pgm;
	{0x85,0xe1},//pin pin_cs;
	{0x95,0xf1},//pin pin_vcc;
	{0x17,0x63},//pin pin_gnd;
	{0x22,0x42},//pin pin_vpp;
	{	{0x13,0x73},{0x12,0x72},{0x11,0x71},{0x10,0x70},{0x27,0x53},{0x26,0x52},{0x25,0x51},{0x24,0x50},
		{0x92,0xc2},{0x91,0xc1},{0x86,0xe2},{0x90,0xc0},{0x23,0x43},{0x93,0xc3},{0x94,0xf0},{0xff,0xff},
		{0xff,0xff},{0xff,0xff},
	},//pin pin_addr[18];
	{	{0x14,0x60},{0x15,0x61},{0x16,0x62},{0x80,0xd0},{0x81,0xd1},{0x82,0xd2},{0x83,0xd3},{0x84,0xe0},
	},//pin pin_data[8];
};

static unsigned char wr_addr_mirror[0x10];
static unsigned char rot_table1[] = {0x03, 0x0C, 0x30, 0xC0};
static unsigned char rot_table2[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void getparams(){
	unsigned char x;
	for(x=0; x<sizeof (eprom_param_t); x++){
		((unsigned char *)&params)[x] = uart_getc();
	}
}

void pin_set_input(xdata pin * mypin){
	unsigned char wr_addr, wr_value, wr_num, wr_mask=0x03;
	
	if(mypin->read == 0xFF) return;
	
	
	wr_addr = mypin->write>>4;
	wr_num = mypin->write & 0x0F;
	
	wr_value = wr_addr_mirror[wr_addr];
	wr_mask = rot_table1[wr_num];
	
	wr_value &= ~wr_mask;
	wr_value |= 0x55 & wr_mask;
	
	wr_addr_mirror[wr_addr] = wr_value;
	
	*( (xdata unsigned char *)(0x800+(wr_addr<<8)) ) = wr_value;
}

void pin_set_special(xdata pin * mypin){
	unsigned char wr_addr, wr_value, wr_num, wr_mask=0x03;
	
	if(mypin->read == 0xFF) return;
	
	
	wr_addr = mypin->write>>4;
	wr_num = mypin->write & 0x0F;
	
	wr_value = wr_addr_mirror[wr_addr];
	wr_mask = rot_table1[wr_num];
	
	wr_value |= wr_mask;
	
	wr_addr_mirror[wr_addr] = wr_value;
	
	*( (xdata unsigned char *)(0x800+(wr_addr<<8)) ) = wr_value;
}

void pin_set_state(xdata pin * mypin, unsigned char state){
	unsigned char wr_addr, wr_value, wr_num, wr_mask=0x03;
	
	if(mypin->read == 0xFF) return;
	
	wr_addr = mypin->write>>4;
	wr_num = mypin->write & 0x0F;
	
	wr_value = wr_addr_mirror[wr_addr];
	wr_mask = rot_table1[wr_num];
	
	wr_value &= ~wr_mask;
	wr_value |= wr_mask & (state?0xAA:0);
	
	wr_addr_mirror[wr_addr] = wr_value;
	
	*( (xdata unsigned char *)(0x800+(wr_addr<<8)) ) = wr_value;
}

unsigned char pin_get_state(xdata pin * mypin){
	unsigned char rd_addr, rd_num, rd_mask=0x01;
	
	if(mypin->read == 0xFF) return 0;
	
	rd_addr = mypin->read>>4;
	rd_num = mypin->read & 0x0F;
	
	rd_mask = rot_table2[rd_num];

	return ( *( (xdata unsigned char *)(0x800+(rd_addr<<8)) ) ) & rd_mask;
}


void activate(){
	unsigned char x;
	
	for(x=0;x<8;x++){
		pin_set_input(&params.pin_data[x]);
	}
	
	pin_set_special(&params.pin_vcc);
	pin_set_state(&params.pin_vpp, 1);
	
	pin_set_special(&params.pin_gnd);
	
	pin_set_state(&params.pin_cs, 1);
	pin_set_state(&params.pin_oe, 1);
	pin_set_state(&params.pin_pgm, 1);
	
	for(x=0;x<params.addrlines;x++){
		pin_set_state(&params.pin_addr[x], 0);
	}
	
}

void deactivate(){
	unsigned char x;
	
	for(x=0;x<params.addrlines;x++){
		pin_set_input(&params.pin_addr[x]);
	}
	
	pin_set_input(&params.pin_cs);
	pin_set_input(&params.pin_oe);
	pin_set_input(&params.pin_pgm);
	
	pin_set_input(&params.pin_gnd);
	
	pin_set_input(&params.pin_vpp);
	pin_set_input(&params.pin_vcc);
}

void eprom_addr_set(unsigned int addr){
	unsigned char x;
	for(x=0;x<params.addrlines;x++){
		pin_set_state(&params.pin_addr[x], addr & 0x0001);
		addr>>=1;
	}
}

unsigned char eprom_data_get(){
	unsigned char x, dat;
	for(x=0;x<8;x++){
		dat>>=1;
		dat |= pin_get_state(&params.pin_data[x])?0x80:0;
	}
	return dat;
}

unsigned char eprom_read(unsigned int addr){
	unsigned char tmp;
	eprom_addr_set(addr);
	pin_set_state(&params.pin_oe,0);
	tmp = eprom_data_get();
	pin_set_state(&params.pin_oe,1);
	return(tmp);	
}

void read(){
	unsigned int addr, len;
	
	addr = uart_getc()<<8;//read start address from uart
	addr |= uart_getc();
	
	len = uart_getc()<<8;//read len from uart
	len |= uart_getc();
	
	activate();	

	pin_set_state(&params.pin_cs, 0);
	while(len--){
		uart_putc(eprom_read(addr++));
	}
	pin_set_state(&params.pin_cs, 1);
	
	deactivate();
}

code static unsigned char setup_regs[] = {0xB0, 0x40, 0x50, 0x70, 0x60, 0x30, 0xf0, 0xc0, 0xe0, 0xd0};

void init(){
	unsigned char x;
	for(x=0;x<sizeof(setup_regs);x++){
		*((xdata unsigned char *)((setup_regs[x]<<4)+0x800)) = 0x55; //set all pins to input
		wr_addr_mirror[setup_regs[x]>>4] = 0x55;
	}	
}

int main(){
	byte c;

	RS0 = 0;
	RS1 = 0;

	//uart_init();
	
	init();
	
	while(1){
		c = uart_getc();
		switch(c){
			case 'p':
				getparams();
				break;
			case 'r':
				read();
				break;
			case 'a':
				activate();
				break;
			case 't':
				//activate();
				eprom_addr_set(0x1234);
				break;
		}
	}
	
}
