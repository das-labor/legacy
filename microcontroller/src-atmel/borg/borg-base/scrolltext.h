typedef struct{
	unsigned char len;
	unsigned char pix[5];
}charmap;

void shift_in(charmap * cm, unsigned int delay);
void alice_putchar(char c);
void alice_putstr(char * str);
//void shift_out(unsigned char cols, unsigned int delay);
