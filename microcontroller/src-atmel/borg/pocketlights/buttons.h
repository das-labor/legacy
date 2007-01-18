#include "fifo.h"

// I/O Pin für die 8 Buttons
#define BUTTONPIN 	3
#define BUTTONDDR	DDRB
#define BUTTONPORT 	PORTB

#define BUF_SIZE 10
extern fifo_t fifo;

// enum für die 8 Buttons. Stimmt mit Pin belegung überein!
// TODO: correct pin numbers
typedef enum
{
BUTTON_UP = 0,
BUTTON_DOWN = 1,
BUTTON_LEFT = 2,
BUTTON_RIGHT = 3,
BUTTON_SELECT = 4,
BUTTON_START = 5, 
BUTTON_B = 6,
BUTTON_A = 7
}
button_value;

enum
{
BUTTON_UP_MASK = 0x01,
BUTTON_DOWN_MASK = 0x02,
BUTTON_LEFT_MASK = 0x04,
BUTTON_RIGHT_MASK = 0x08,
BUTTON_SELECT_MASK = 0x10,
BUTTON_START_MASK = 0x20, 
BUTTON_B_MASK = 0x40,
BUTTON_A_MASK = 0x80
}
button_mask;

void init_buttons();
void button_register(unsigned char button_pin_state);
void button_add(button_value button);
button_value get_button();
