
void stepper_init();


void update_stepper();

extern uint8_t stepper_command;
#define CMD_STEP    1
#define CMD_RUN     2
#define CMD_HOME    3
#define CMD_GOTO    4

extern uint8_t stepper_run;
//1= forward, 0= reverse
extern uint8_t stepper_dir;

extern uint16_t position_request;