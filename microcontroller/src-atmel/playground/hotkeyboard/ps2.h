




typedef enum{
	n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, A, B, C, D, E, F, G, H, I, J, K, alt, ctrl, escape
}ps2_key;

#ifdef PS2_C
static unsigned char key_table[] PROGMEM = {
	0x45,//0
	0x16,//1
	0x1E,//2
	0x26,//3
	0x25,//4
	0x2E,//5
	0x36,//6
	0x3D,//7
	0x3E,//8
	0x46,//9
	0x1c,//A
	0x32,//B
	0x21,//C
	0x23,//D
	0x24,//E
	0x2B,//F
	0x34,//G
	0x33,//H
	0x43,//I
	0x3B,//J
	0x42,//K
	0x11,//lt alt
	0x14,//lt ctrl
	0x76,//escape
};
#endif

void key_press(ps2_key key);
void key_make(ps2_key key);
void key_break(ps2_key key);
void init_ps2();

void ps2_put(unsigned char size, unsigned char byte1, unsigned char byte2);
unsigned char ps2_get_nb(unsigned char * byte);


