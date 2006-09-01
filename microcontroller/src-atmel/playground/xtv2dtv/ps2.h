


//void key_press(ps2_key key);
void key_make(uint16_t data);
void key_break(uint16_t data);
void init_ps2();

void ps2_put(unsigned char size, unsigned char byte1, unsigned char byte2);
unsigned char ps2_get_nb(unsigned char * byte);


