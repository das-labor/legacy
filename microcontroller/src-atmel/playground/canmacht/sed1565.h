#define DISPSETPAGE(y) dispWriteCommand(0xB0|(y))
#define DISPLINESTART(start) dispWriteCommand(((start) & 63) | 64)

void dispInit(void);         //Initialize LCD
void dispClear(void);

void dispGotoXY(unsigned char Spalte, unsigned char Zeile);

void dispSetPix(uint8_t x, uint8_t y, uint8_t color);

void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color);
void dispVLine(uint8_t x, uint8_t y, uint8_t y2);

void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

