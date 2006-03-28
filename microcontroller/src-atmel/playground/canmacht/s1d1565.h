extern void dispWriteCommand(unsigned char byte);
extern void dispWriteByte(unsigned char byte);
extern void dispInit(void);         //Initialize LCD
extern void dispClear(void);
extern void dispGotoXY(unsigned char Spalte, unsigned char Zeile);
extern void dispEnable(void);
extern unsigned char dispReadByte(void);

