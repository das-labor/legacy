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
BUTTON_START = 4, 
BUTTON_SELECT = 5,
BUTTON_A = 6,
BUTTON_B = 7,
}
which_button;

void init_buttons();
void button_action(int button);