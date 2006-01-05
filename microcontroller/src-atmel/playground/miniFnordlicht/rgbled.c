
#include <inttypes.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/sleep.h>

#define    PWM11        1
#define    PWM10        0

#define PIN_RED 1
#define PIN_GREEN 3
#define PIN_BLUE 5


uint16_t timeTable [] = { 0, 0, 0, 0, 0, 0, 0, 0, 210, 56, 62, 69, 75, 82, 89, 95, 102, 108, 115, 121, 128, 
 								  135, 141, 148, 154, 161, 167, 174, 180, 187, 194, 200, 207, 
								  213, 220, 226, 233, 239, 246, 253, 259, 266, 272, 279, 285, 
								  292, 299, 305, 312, 318, 325, 331, 338, 344, 351, 358, 364, 
								  371, 377, 384, 390, 397, 404, 410, 417, 423, 430, 436, 443, 
								  449, 456, 463, 469, 476, 482, 489, 495, 502, 509, 515, 522, 
								  528, 535, 541, 548, 554, 561, 568, 574, 581, 587, 594, 600, 
								  607, 614, 620, 627, 633, 640, 646, 653, 659, 666, 673, 679, 
								  686, 692, 699, 705, 712, 718, 725, 732, 738, 745, 751, 758, 
								  764, 771, 778, 784, 791, 797, 804, 810, 817, 823, 830, 837, 
								  843, 850, 856, 863, 869, 876, 883, 889, 896, 902, 909, 915, 
								  922, 928, 935, 942, 948, 955, 961, 968, 974, 981, 988, 994, 
								  1001, 1007, 1014, 1020, 1027, 1033, 1040, 1047, 1053, 1060, 
								  1066, 1073, 1079, 1086, 1093, 1099, 1106, 1112, 1119, 1125, 
								  1132, 1138, 1145, 1152, 1158, 1165, 1171, 1178, 1184, 1191, 
								  1197, 1204, 1211, 1217, 1224, 1230, 1237, 1243, 1250, 1257, 
								  1263, 1270, 1276, 1283, 1289, 1296, 1302, 1309, 1316, 1322, 
								  1329, 1335, 1342, 1348, 1355, 1362, 1368, 1375, 1381, 1388, 
								  1394, 1401, 1407, 1414, 1421, 1427, 1434, 1440, 1447, 1453, 
								  1460, 1467, 1473, 1480, 1486, 1493, 1499, 1506, 1512, 1519, 
								  1526, 1532, 1539, 1545, 1552, 1558, 1565, 1572, 1578, 1585, 
								  1591, 1598, 1604, 1611, 1617, 1624, 1631, 1637, 1644, 1650, 
								  1657, 1663, 1670
								};

uint8_t color[3] = {255, 0, 0}; 


SIGNAL(SIG_OUTPUT_COMPARE1A) {
	static uint8_t bright = 8;
	TCNT1 = 0;
	OCR1A  = timeTable[++bright];

	if (bright < color[0]) { // red
		PORTB |= (1 << PIN_RED);
	} else {
	    PORTB &= ~(1 << PIN_RED);
	}
	if (bright < color[1]) { // green
		PORTB |= (1 << PIN_GREEN);
	} else {
		PORTB &= ~(1 << PIN_GREEN);
	} 
	if (bright < color[2]) { // blue
		PORTB |= (1 << PIN_BLUE);
	} else {
		PORTB &= ~(1 << PIN_BLUE);
	}
	if(bright==255)
		bright=8;
}


void timer1_on() {
	TCCR1A = 0; 			
	TCCR1B = 1;		// Counter clk
	OCR1A  = timeTable[8];		
	TCNT1  = 0;
	TIFR   = (1 << OCF1A);
	TIMSK  = (1 << OCIE1A);
	sei();
	
}

void wait(int ms){
	//uint16_t i = 0xffff;
/* 	TCCR2: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
		CS22 CS21 CS20
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/32
		 1    0    0       clk/64
		 1    0    1       clk/128
		 1    1    0       clk/256
		 1    1    1       clk/1024	
*/
	TCCR2 = 0x0D;			//CTC Mode, clk/128
	OCR2 = (F_CPU/128000);	//1000Hz 
	for(;ms>0;ms--){
		while(!(TIFR&0x80));//while(i--) asm volatile ("nop");	//wait for compare match flag
		TIFR=0x80;			//reset flag
	}

}

void fadeToColor(uint8_t *fadeColor, uint8_t steps) {
	uint8_t i;
	int16_t helpColor[3], addColor[3];
	
	helpColor[0] = color[0]*128;
	helpColor[1] = color[1]*128;
	helpColor[2] = color[2]*128;
	
	addColor[0] = ((fadeColor[0]*128)-helpColor[0])/steps;
	addColor[1] = ((fadeColor[1]*128)-helpColor[1])/steps;
	addColor[2] = ((fadeColor[2]*128)-helpColor[2])/steps;
	
	for (i = 0; i < steps; i++) {
		helpColor[0] += addColor[0];
		helpColor[1] += addColor[1];
		helpColor[2] += addColor[2];
		color[0] = helpColor[0]/128;
		color[1] = helpColor[1]/128;
		color[2] = helpColor[2]/128;
		wait(4);
	}		
}

int main(void) {

	// Setzt das Richtungsregister des Ports B auf 0xff 
	//(alle Pins als Ausgang):
	DDRB = 0xff;
	// Setzt PortB auf : 
	PORTB = 0x00;
	uint8_t i;
	timer1_on();

	while(1) {
		for (i = 0; i < 255; i++) {
			color[0] = 255-i;
			color[1] = i;
			wait(4);
		} 			
		for (i = 0; i < 255; i++) {
			color[1] = 255-i;
			color[2] = i;
			wait(4);
		}
		for (i = 0; i < 255; i++) {
			color[2] = 255-i;
			color[0] = i;
			wait(4);
		}

	}
	  
}
