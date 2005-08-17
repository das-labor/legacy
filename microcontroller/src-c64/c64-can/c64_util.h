/*
	Some usefull makros
*/


#define outb(addr,byte) (*((unsigned char *)addr) = byte)
#define inb(addr) *((unsigned char *)addr)

#define REG(addr) *((unsigned char*)addr)

#define CIA1 0xdc00
#define CIA2 0xdd00

#define CIA_PRA 0x00
#define CIA_PRB 0x01
#define CIA_DDRA 0x02
#define CIA_DDRB 0x03
#define CIA_TALO 0x04
#define CIA_TAHI 0x05
#define CIA_TBLO 0x06
#define CIA_TBHI 0x07
#define CIA_TOD10THS 0x08
#define CIA_TODSEC 0x09
#define CIA_TODMIN 0x0A
#define CIA_TODHR 0x0B
#define CIA_SDR 0x0C
#define CIA_ICR 0x0D
#define CIA_CRA 0x0E
#define CIA_CRB 0x0F
