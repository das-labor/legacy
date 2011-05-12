
//Stepper pins on Port A 0-3
#define STEPPER_PORT PORTA
#define STEPPER_DDR  DDRA

//Lichtschranke
#define LS_PORT     B
#define LS_BIT      2

#define LS_LED_PORT B
#define LS_LED_BIT  1

//rote LED
#define LED_PORT    B
#define LED_BIT     0

//Scanner motor
#define MOTOR_PORT        D
#define MOTOR_BIT         2

#define MOTOR_READY_PORT  D
#define MOTOR_READY_BIT   3


//Laser (OC1A)
#define LASER_PORT        D
#define LASER_BIT         5

//Laser power setting voltage (OC0)
#define LASER_PWM_PORT    B
#define LASER_PWM_BIT     3

//Door Switch (needs pullup, low means door closed)
#define DOOR_SWITCH_PORT  C
#define DOOR_SWITCH_BIT   0
