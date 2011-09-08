
#define ERROR_NONE 0		//no error occured
#define ERROR_SR_TOHIGH 1	//samplerate is to high, xmega cant calc in time

#define ERROR_blink_short 1
#define ERROR_blink_long 0
#define ERROR_MASK 3		//3bits

void ERROR_blink();
void setERROR(uint8_t error);

uint8_t getERROR(void);
void clearERROR(void);