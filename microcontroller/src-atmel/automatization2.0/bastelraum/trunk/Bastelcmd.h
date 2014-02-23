#define PORT_BASTEL		1

#define C_SW			0
#define C_SW_MASTER		2
#define C_PWM			1
#define C_PWM_MASTER	3
#define C_SET_MOTION_T	10
#define C_GET_STATE		11

typedef enum {
	F_REG_UNUSED,
	F_REG_BTISCHR_1,
	F_REG_BTISCHR_2,
	F_REG_BTISCHL_1,
	F_REG_BTISCHL_2,
	F_REG_FENSTER,
	F_REG_MITTE,
	F_REG_NISCHE,
	F_REG_COUNT
} t_sreg_funct_def;

typedef enum {
	F_PWM_FENSTER,
	F_PWM_MITTE,
	F_PWM_NISCHE,
	F_PWM_UNUSED,
	F_PWM_COUNT
} f_pwm_funct_def;

typedef enum {
	RGBLED_R,
	RGBLED_G,
	RGBLED_B,
	LED_1,
	LED_2,
	LED_3
} f_uart_led_def;

typedef enum {
	TASTER_LICHT,
	TASTER_R,
	TASTER_G,
	TUERKONTAKT
} f_uart_input_def;
