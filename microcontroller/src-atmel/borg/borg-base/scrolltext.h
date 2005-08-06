
typedef struct{
	unsigned char len;
	unsigned char pix[8];
}charmap;

//void shift_in(charmap * cm, unsigned int delay);
void shift_out(unsigned char cols, unsigned int delay);

void labor_borg();
