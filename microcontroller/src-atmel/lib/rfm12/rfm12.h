//rfm12 public header file





//function protoypes
void rfm12_init();
void rfm12_data(uint16_t d);
void rfm12_tx(uint8_t tape, uint8_t length, uint8_t *data);
uint16_t rfm12_read(uint16_t d);
