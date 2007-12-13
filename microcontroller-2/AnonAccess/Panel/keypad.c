#include "config.h"
#include "lcd_tools.h"

char read_keypad(void){

	//Variablen
	uint8_t zeichen = 0x10;
	uint8_t i, j;
	uint8_t rowx[4], collx[4];

	//defines Uebergeben
	rowx[0] = ROW1;
	rowx[1] = ROW2;
	rowx[2] = ROW3;
	rowx[3] = ROW4;
	collx[0] = COLL1;
	collx[1] = COLL2;
	collx[2] = COLL3;
	collx[3] = COLL4;

	//Zeilen und Spalten abfragen
	for (i=0;i <= 3;i++){
		DDR_ROWS |= (1 << rowx[i]);
		for(j=0;j<=3;j++){
			if ((PIN_COLLS & (1 << collx[j])) == 0){
				zeichen = j + (i * 4);
			}
		}
		DDR_ROWS &= ~(1 << rowx[i]);
	}
	
	zeichen=(zeichen>0xF)?0x10:zeichen;
	char tab[]="0123456789ABCDEF ";

	return tab[zeichen];
}

void keypad_init(void){

	PORT_COLLS |= (1 << COLL1) | (1 << COLL2) | (1 << COLL3) | (1 << COLL4);

}
