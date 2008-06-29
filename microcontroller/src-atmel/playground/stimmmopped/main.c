#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <math.h>

#define LED_PORT PORTB
#define LED0 PB0
#define LED1 PB1
//          bits 543210 
#define PORT_SEG_GFABCP PORTC
#define PORT_SEC_xxxxDE PORTD

#define PIN_TASTEN  PIND
#define PORT_TASTEN PORTD
#define TASTE1 PD3
#define TASTE2 PD2

#define C          0
#define C_SHARP    1
#define D          2
#define D_SHARP    3
#define E          4
#define F          5
#define F_SHARP    6
#define G          7
#define G_SHARP    8
#define A          9
#define A_SHARP   10
#define B         11

//#define NOTE(n, o)   (n + (12 * (o + 1)))
//#define WURZEL12_2  1.05946309436f

#define TIME_INAKIVE  100*30 // 30 Sekunden

unsigned char tasten_timer[2];
unsigned char timer1_corrcet = 0;
unsigned short timer1_max;

unsigned char taste1_ent, taste2_ent, taste1_sf, taste2_sf; // ff flase
#define TIME_DEBOUNCE   3    // 80 ms
#define TIME_NOTE     150
#define TIME_OCTAVE    70

unsigned char logicTimer   = 0;
unsigned char displayTimer = 0;
unsigned char displayState = 0;

unsigned long timeInaktive;

#define SEG_A	3
#define SEG_B   2
#define SEG_C   1
#define SEG_D   1
#define SEG_E   0
#define SEG_F   4
#define SEG_G   5
#define SEG_P   0

typedef struct {
	unsigned char seg_a:1;
	unsigned char seg_b:1;
	unsigned char seg_c:1;
	unsigned char seg_d:1;
	unsigned char seg_e:1;
	unsigned char seg_f:1;
	unsigned char seg_g:1;
	unsigned char seg_p:1;
} display_t; 

typedef enum {
	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	TONE_C,
	TONE_C_SHARP,
	TONE_D,
	TONE_D_SHARP,
	TONE_E,
	TONE_F,
	TONE_F_SHARP,
	TONE_G,
	TONE_G_SHARP,
	TONE_A,
	TONE_A_SHARP,
	TONE_B,
	ENDMARK 
} chars_t;

chars_t display_tone = TONE_C;
chars_t display_octave = NUM_0;

display_t display[] = {
{1, 1, 1, 1, 1, 1, 0, 0},  // NUM_0,
{0, 1, 1, 0, 0, 0, 0, 0},  // NUM_1,
{1, 1, 0, 1, 1, 0, 1, 0},  // NUM_2,
{1, 1, 1, 1, 0, 0, 1, 0},  // NUM_3,
{0, 1, 1, 0, 0, 1, 1, 0},  // NUM_4,
{1, 0, 1, 1, 0, 1, 1, 0},  // NUM_5,
{1, 0, 1, 1, 1, 1, 1, 0},  // NUM_6,
{1, 1, 1, 0, 0, 0, 0, 0},  // NUM_7,
{1, 1, 1, 1, 1, 1, 1, 0},  // NUM_8,
{1, 1, 1, 1, 0, 1, 1, 0},  // NUM_9,
{1, 0, 0, 1, 1, 1, 0, 0},  // TONE_C,
{1, 0, 0, 1, 1, 1, 0, 1},  // TONE_C_SHARP,
{0, 1, 1, 1, 1, 0, 1, 0},  // TONE_D,
{0, 1, 1, 1, 1, 0, 1, 1},  // TONE_D_SHARP,
{1, 0, 0, 1, 1, 1, 1, 0},  // TONE_E,
{1, 0, 0, 0, 1, 1, 1, 0},  // TONE_F,
{1, 0, 0, 0, 1, 1, 1, 1},  // TONE_F_SHARP,
{1, 0, 1, 1, 1, 1, 0, 0},  // TONE_G,
{1, 0, 1, 1, 1, 1, 0, 1},  // TONE_G_SHARP,
{1, 1, 1, 0, 1, 1, 1, 0},  // TONE_A,
{1, 1, 1, 0, 1, 1, 1, 1},  // TONE_A_SHARP,
{0, 0, 1, 1, 1, 1, 1, 0},  // TONE_B,
};

unsigned long midi_freq_tab[108] = {
	489249, 461789, 435871, 411408, 388317, 366522, 345951, 326534, 308207,
	290909, 274582, 259171, 244624, 230895, 217936, 205704, 194159, 183261,
    172976, 163267, 154104, 145455, 137291, 129585, 122312, 115447, 108968,
	102852,  97079,  91631,  86488,  81634,  77052,  72727,  68645,  64793,
	 61156,  57724,  54484,  51426,  48540,  45815,  43244,  40817,  38526,
     36364,  34323,  32396,  30578,  28862,  27242,  25713,  24270,  22908,
	 21622,  20408,  19263,  18182,  17161,  16198,  15289,  14431,  13621,
     12856,  12135,  11454,  10811,  10204,   9631,   9091,   8581,   8099,
      7645,   7215,   6810,   6428,   6067,   5727,   5405,   5102,   4816,
	  4545,   4290,   4050,   3822,   3608,   3405,   3214,   3034,   2863,
	  2703,   2551,   2408,   2273,   2145,   2025,   1911,   1804,   1703,
	  1607,   1517,   1432,   1351,   1276,   1204,   1136,   1073,   1012
};


unsigned char timer;

ISR(TIMER1_COMPA_vect) {
	switch (timer) {
		case 1:
			LED_PORT |= (1 << LED0);
			break;
			
		case 2:
			LED_PORT &= ~(1 << LED0);
			break;
			
		case 6:
			LED_PORT |= (1 << LED1);
			break;
			
		case 7:
			LED_PORT &= ~(1 << LED1);
			break;

		case 10:
			timer = 0;
			break;
	}
	if (timer < timer1_corrcet)
		TCNT1 -= 1;
	timer++;
}

ISR(TIMER0_COMPA_vect) {
	logicTimer++;
}

ISR(INT1_vect) {
	
}

void calcFreqAndNotename(unsigned char note) {
	unsigned long freq;
	//freq = (float) F_CPU/(440.*pow(WURZEL12_2, note-69.));
	//freq = (float) F_CPU/(440.f*pow(2.f, (note-69.f)/12.f));
	if (note >= 12 && note < 120)
		freq = midi_freq_tab[note - 12];
	OCR1A = freq / 10;
	timer1_corrcet = freq % 10;
	displayState = 0;
	displayTimer = TIME_NOTE;
	display_tone   = (note % 12) + TONE_C;
	display_octave = ((note - 12) / 12) + NUM_0; 
}

void sleepAndWakeUpControl() {
	unsigned short tim = 900;
	PORTB = 0x00;
	PORTC = 0xFF;
	PORTD = 0xFF;
	EIMSK = 2;
	while (tim) {
		sleep_enable();
		LED_PORT = 0;
		sleep_cpu();
		sleep_disable();
		tim--;
	}					
	cli();
	EIMSK = 0;
	sei(); 
}

static unsigned char timer_pwr_dwn;
static unsigned char taste1, taste2;
void tastenEntprellen() {
	unsigned char input;
	input = (PIN_TASTEN & (1 << TASTE1)) == 0;
	if (taste1 != input) {
		taste1 = input;
		timer_pwr_dwn = 250;
		tasten_timer[0] = TIME_DEBOUNCE;
	} else {
		if (tasten_timer[0]) {
			tasten_timer[0]--;
		} else {
			if (taste1_ent == 0 &&
			    taste1 == 1) {
				taste1_sf = 1;
				timeInaktive = TIME_INAKIVE;
			}
			taste1_ent = taste1;
		}
		if (input) {
			if (timer_pwr_dwn) {
				timer_pwr_dwn--;
			} else {
				sleepAndWakeUpControl(); 
				timer_pwr_dwn = 250;
			}
		}
	}
	input = (PIN_TASTEN & (1 << TASTE2)) == 0;
	if (taste2 != input) {
		taste2 = input;
		tasten_timer[1] = TIME_DEBOUNCE;
	} else {
		if (tasten_timer[1]) {
			tasten_timer[1]--;
		} else {
			if (taste2_ent == 0 &&
			    taste2 == 1) {
				taste2_sf = 1;
				timeInaktive = TIME_INAKIVE;
			}
			taste2_ent = taste2;
		}
	}
	
	if (timeInaktive) {
		timeInaktive--;
	} else {
		sleepAndWakeUpControl();
		timeInaktive = TIME_INAKIVE;
	}
	
}

typedef enum {
	ML_MAIN,
	ML_SUB,
} menu_level_t;

typedef enum {
	MR_BASS,
	MR_BASS_DSHARP,
	MR_GUITAR,
	MR_CUSTOM
} menu_root_t;

void setDisplay(chars_t ch) { 
	display_t dis = display[ch];
	PORT_SEG_GFABCP = (!dis.seg_a << SEG_A) | 
	                  (!dis.seg_b << SEG_B) |
					  (!dis.seg_c << SEG_C) | 
					  (!dis.seg_f << SEG_F) |
					  (!dis.seg_g << SEG_G) | 
					  (!dis.seg_p << SEG_P);
	if (!dis.seg_e)
		PORT_SEC_xxxxDE |=  (1 << SEG_E);
	else
		PORT_SEC_xxxxDE &= ~(1 << SEG_E);
	if (!dis.seg_d)
		PORT_SEC_xxxxDE |=  (1 << SEG_D);
	else
		PORT_SEC_xxxxDE &= ~(1 << SEG_D);
}

void showCurrentNote() {
	switch (displayState) {
		case 0:
			setDisplay(display_tone);
			if (displayTimer) {
 				displayTimer--;
			} else { 
				displayTimer = TIME_OCTAVE;
				displayState = 1;
			}
			break;
			
		case 1:
			setDisplay(display_octave);
			if (displayTimer) {
 				displayTimer--;
			} else { 
				displayTimer = TIME_NOTE;
				displayState = 0;
			}
			break;
	}
}


// 0 markiert das ende
unsigned char midi_notes_bass[] = {
	23, 26, 28, 33, 38, 43, 0
};

unsigned char midi_notes_bass_dsharp[] = {
	22, 27, 32, 37, 42, 0
};


unsigned char midi_notes_guitar[] = {
	38, 40, 45, 50, 55, 59, 64, 0
};

unsigned char midi_notes_custom[] = {
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
	22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
	38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
	54, 55, 56, 57, 58, 59, 60, 61, 0
};

menu_level_t  menu_level = ML_MAIN;
menu_root_t   menu_root = 0;

unsigned char* note_ptr;
unsigned char  sub_index  = 0, old_sub_index = 1;

void stateMachineMenu() {
	switch (menu_level) {
		case ML_MAIN:
			switch (menu_root)
			{
				case MR_BASS: // Bass
					setDisplay(TONE_B);
					note_ptr = midi_notes_bass;
					if (taste1_sf)
						menu_root = MR_BASS_DSHARP;
					break;
					
				case MR_BASS_DSHARP: // Bass D-Sharp
					setDisplay(TONE_D_SHARP);
					note_ptr = midi_notes_bass_dsharp;
					if (taste1_sf)
						menu_root = MR_GUITAR;
					break;
										
				case MR_GUITAR: // Guitar
					setDisplay(TONE_G);
					note_ptr = midi_notes_guitar;
					if (taste1_sf)
						menu_root = MR_CUSTOM;
					break;
				
				case MR_CUSTOM: // Custom
					setDisplay(TONE_C);
					note_ptr = midi_notes_custom;
					if (taste1_sf) 
						menu_root = MR_BASS;
					break;
			}
			sub_index = 0;
			old_sub_index = 1;
			if (taste2_sf) 
				menu_level = ML_SUB;
			break;
			
		case ML_SUB:
			if (sub_index != old_sub_index) {
				old_sub_index = sub_index;
				calcFreqAndNotename(note_ptr[sub_index]);
			} else {
				showCurrentNote();
			}
			if (taste1_sf) {
				if (!note_ptr[sub_index + 1])
					sub_index = 0;
				else 
					sub_index++;
			} else if (taste2_sf) {
				menu_level = ML_MAIN;
			}
			break;
	}
	taste1_sf = 0;
	taste2_sf = 0;
}
 
// WGM13-WGM10 auf 0 (Mode 0)
// CS10 auf 1  no prescaler
void main() {
    EIMSK   = 0;
	TCCR0A  = 2;  // CTC
	TCCR0B  = 3;  // prescaler 64
	OCR0A   = 125;	
	TIMSK0 |= (1 << OCIE0A);
	TCCR1B  = 9; // CTC prescaler 1
	TIMSK1 |= (1 << OCIE1A);
	OCR1A   = 19416;
	timer1_corrcet =     2;
	timer1_max     = 19416;
	timeInaktive   = TIME_INAKIVE;
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	DDRB    = 0xFF;
	PORTB   = 0;
	DDRC    = 0xFF;
	PRR     = (1 << PRTWI) | (1 << PRTIM2) | (1 << PRSPI) | (1 << PRUSART0) | (1 << PRADC); 
	//SMCR    = 5;
	DDRD    = (1 << LED0) | (1 << LED1);
	PORT_TASTEN = (1 << TASTE1) | (1 << TASTE2);
	timer  = 0;
	sei();
	
	setDisplay(NUM_0);
	
	while (1) {
		if (logicTimer > 9) {
			logicTimer = 0;
			tastenEntprellen(); 
			stateMachineMenu();
		}
	}
}
