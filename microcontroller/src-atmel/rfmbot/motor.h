#include <avr/io.h>

#define RB_EN_L_BIT   (_BV(PD4))
#define RB_EN_L_DRR   (DDRD)
#define RB_EN_L_PORT  (PORTD)

#define RB_EN_R_BIT   (_BV(PD5))
#define RB_EN_R_DRR   (DDRD)
#define RB_EN_R_PORT  (PORTD)

#define RB_FW_L_BIT   (_BV(PC0))
#define RB_FW_L_DRR   (DDRC)
#define RB_FW_L_PORT  (PORTC)

#define RB_FW_R_BIT   (_BV(PC3))
#define RB_FW_R_DRR   (DDRC)
#define RB_FW_R_PORT  (PORTC)

#define RB_RW_L_BIT   (_BV(PC1))
#define RB_RW_L_DRR   (DDRC)
#define RB_RW_L_PORT  (PORTC)

#define RB_RW_R_BIT   (_BV(PD7))
#define RB_RW_R_DRR   (DDRD)
#define RB_RW_R_PORT  (PORTD)

#define RB_DIR_FW     0x10
#define RB_DIR_RW     0x20

#define RB_LEFT       0x01
#define RB_RIGHT      0x02

#define IDX_R         0x00
#define IDX_L         0x01

typedef struct
{
	uint8_t dststate;
	uint8_t cstate;
} motor_state_t;

void motor_ctrl_direct (uint8_t in_val);
void motor_init (void);
void motor_pwm (void);
void motor_ctrl (uint8_t in_val);
