
#define METHOD_eprom_std 0
#define METHOD_eprom_oevpp 1
#define METHOD_eprom_pgmpin 2
#define METHOD_eeprom_std 10


typedef struct{
	unsigned char read;
	unsigned char write;
}pin;

typedef struct{
	unsigned char addrlines;
	unsigned char vpp;
	uint16_t pgm_time;
	unsigned char pgm_cycles;
	unsigned char flags;
	unsigned char method;
	pin pin_oe;
	pin pin_pgm;
	pin pin_cs;
	pin pin_vcc;
	pin pin_gnd;
	pin pin_vpp;
	pin pin_addr[18];
	pin pin_data[8];
}eprom_param_t; 
