// I/O Pin für die 8 Buttons
#define BUTTONPIN 	3
#define BUTTONDDR	DDRB
#define BUTTONPORT 	PORTB

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
button;

void init_buttons();
void button_record(int button);
