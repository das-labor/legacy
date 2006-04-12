#define DISPSETPAGE(y) dispWriteCommand(0xB0|(y))
#define DISPLINESTART(start) dispWriteCommand(((start) & 63) | 64)

extern void dispWriteCommand(unsigned char byte);
extern void dispWriteByte(unsigned char byte);
extern void dispInit(void);         //Initialize LCD
extern void dispClear(void);
extern void dispGotoXY(unsigned char Spalte, unsigned char Zeile);
extern void dispEnable(void);
extern unsigned char dispReadByte(void);

extern void dispSetPix(uint8_t x, uint8_t y, uint8_t color);
extern void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color);
extern void dispVLine(uint8_t x, uint8_t y, uint8_t y2);
extern void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
extern void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
