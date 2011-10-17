#define SWITCH_ON  0x0100
#define SWITCH_OFF 0x0000

void rc_switch_set_p(uint16_t param);
void rc_switch_set(uint8_t num, uint8_t state);

void can_switch_set(uint8_t device, uint8_t num, uint8_t state);

void lampedim(uint16_t param);
void all_lampedim(uint16_t param);

void lamp_and_switch_init();

#define LAMP_COUCHFLUTER 0
#define LAMP_HINTENFLUTER 1
#define LAMP_MOODBAR 2
#define LAMP_BASTELECKE 3
//#define POWER_BASTELECKE 4
#define POWER_COUCH 5
#define POWER_BAR 6
#define POWER_FLIPPER 7
#define LAMP_LEUCHTSTOFF1 20
#define LAMP_LEUCHTSTOFF2 21

//not connected to this device
//connected via can to device 0x53

#define POWER_BASTELECKE 21
#define LAMP_LEUCHTSTOFF3 20


#define NUM_RELAIS 2

extern uint8_t relais[2];


#ifdef LAMP_AND_SWITCH_C

uint32_t rc_switch_codes[12][2] PROGMEM={
	{ 0x01000040 /*Fluter aus*/,				0x010000C0 /*Fluter an*/			},
	{ 0x00144515 /*Hinten Fluter aus*/,			0x00154515 /*Hinten Fluter an*/		},
	{ 0x00140515 /*Theke aus*/,					0x00150515 /*Theke an*/				},
	{ 0x00140555 /*Bastelecken Licht aus*/,		0x00150555 /*Bastelecken Licht an*/	},
	{ 0x00541400 /*Steckdose C (Bastelecke)*/,	0x00511400	},
	{ 0x00540500 /*Steckdose A (Couch)*/,		0x00510500	},
	{ 0x00141500 /*Steckdose D (Bar)*/,			0x00111500	},
	{ 0x00010000 /*Steckdose REV(Flipperecke)*/,0x00040000	},
	{ 0x00541101, 0x00511101 }, /* reaktorraum B an/aus */
	{ 0x00541401, 0x00511401 }  /* reaktorraum C an/aus */
};
#else
AVRX_EXT_FIFO(lapd_fifo);
AVRX_EXTERNTASK(lapd_task);
#endif

