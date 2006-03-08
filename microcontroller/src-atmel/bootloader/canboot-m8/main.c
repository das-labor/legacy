#include <avr/io.h>
int bootloader(void);

int main(){
	//DDRC = 0xFF;
	//PORTC = 0x01;
	bootloader();
}
