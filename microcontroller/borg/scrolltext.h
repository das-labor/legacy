
typedef struct{
	unsigned char len;
	unsigned char pix[8];
}charmap;

void shift_in(charmap * cm, unsigned int delay);
void shift_out(unsigned char cols, unsigned int delay);

#ifndef SCROLLTEXT_C
extern charmap pix_L;
extern charmap pix_A;
extern charmap pix_B;
extern charmap pix_O;
extern charmap pix_R;
extern charmap pix_G;
extern charmap pix_W;



#endif
