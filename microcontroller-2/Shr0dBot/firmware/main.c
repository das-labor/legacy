#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <stdio.h>
#include <string.h>

#include "hgrobo_config.h"
#include "hgrobo/motor.h"
#include "hgrobo/encoder.h"
#include "hgrobo/MotorPID.h"
#include "rfm12.h"

//scheduler timings
#define SCHEDINT_DRIVE		0x0800
#define SCHEDINT_MOVE		0x0400
#define SCHEDINT_RADIO		0x2000

//various defines
#define MSG_ALIVE "Shr0dBot alive\r\n"
#define SPEEDHISTDEPTH 4
#define min(a, b) (((a) < (b))?(a):(b))
#define SCHED(x) (robo_sysCnt - (x)) //scheduler time-delta helper function

//drive pid default values
#define DPID_PGAIN 8
#define DPID_IGAIN 60
#define DPID_DGAIN 6

//globals (speed related)
int16_t		robo_curSpeed;		//the robots current speed
int16_t		robo_targSpeed;		//the robots target speed (for PID control)
int16_t		robo_driveOut;		//the driving motor's pwm value
MPID_State	robo_drivePID;		//dive PID controller

//globals (acc sensor)
int16_t		acc_curX;			//current x acceleration
int16_t		acc_curY;			//current y acceleration
int16_t		acc_curZ;			//current z acceleration

//globals (other)
uint32_t	robo_sysCnt;		//a simple system counter
uint16_t	robo_battVolts;		//battery voltage
uint16_t	robo_currentDrain;	//motor current usage

//scratch
int32_t move_encTarg;
int16_t move_setSpeed;
uint8_t move_mode;
#define MOVE_MODE_GOTO 2
#define MOVE_MODE_FREE 1
#define MOVE_MODE_STOP 0

//temporary solution
typedef struct
{
	int16_t		acc_curX;			//current x acceleration
	int16_t		acc_curY;			//current y acceleration
	int16_t		acc_curZ;			//current z acceleration
	uint16_t	battVolts;		//battery voltage
	uint16_t	currentDrain;	//motor current usage
	int16_t		curSpeed;		//the robots current speed	
	int32_t		encAPos;		//the encoder A count
} radio_sensorData;

#define RADIO_TYPE_ROBO 0x42
#define RADIO_CMD_STOP 0x01
#define RADIO_CMD_GOTO 0x02

//debug
int16_t pid;

///// NOTES:
// MOT1 = DRIVE
// MOT2 = STEER; FWD = RIGHT, RWD = LEFT


/* ------------------------------------------------------------------------- */

//setup timer interrupt
void timer0_init()
{
	//reset system counter
	robo_sysCnt = 0;

	//setup timer
	TCCR0 = _BV(CS00); //clk / 1 (int call @ clk/256)
	TCNT0 = 0; //reset counter
	TIMSK |= _BV(TOIE0); //timer 0 overflow int on
	TIFR |= _BV(TOV0); //clear overflow flag
}


//setup all pid controllers with their default values
void pid_init()
{
	robo_drivePID.pGain = DPID_PGAIN;
	robo_drivePID.iGain = DPID_IGAIN;
	robo_drivePID.dGain = DPID_DGAIN;
}


//adc stuff
#define ADMUX_REFS (_BV(REFS0) | _BV(REFS1))		//internal 2,56V voltage reference selection, right-adjust conversion results
uint8_t adc_chsel;									//adc channel selector

//adc start conversion
//this performs a read-modify-write operation -> int flag will reset itself -> pending interrupt lost
#define ADC_SC (ADCSRA |= _BV(ADSC))

//setup adc mux to sample selected channel
inline void adc_mux(void)
{
	//use default input channel
	ADMUX = ADMUX_REFS | (adc_chsel & 0x0F);
}


//setup adc operations
void init_adc(void)
{
	//default to channel 0
	adc_chsel = 0;

	//set adc mux default settings (see header)
	adc_mux();

	//select adc auto trigger source: free running
	SFIOR &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));

	//set adc enable, start conversion, no auto trigger, set interrupt enable, set prescaler
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIE) | (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
}

//IF THE CAP IS LEFT, THEN UP IS X AND LEFT IS Y

//the adc interrupt is a software soc chain
//(to ensure proper channel switching)
ISR(ADC_vect, ISR_NOBLOCK)
{
	switch(adc_chsel)
	{
		//this is the battery voltage level channel
		case 0:
			//save sample
			robo_battVolts = ADC;

			//switch channel
			adc_chsel++;

			//setup adc mux to sample next channel and SC
			adc_mux();
			ADC_SC;
			break;
			
		case 1:
			//save sample
			robo_currentDrain = ADC;

			//switch channel
			adc_chsel = 5;

			//setup adc mux to sample next channel and SC
			adc_mux();
			ADC_SC;
			break;


		//this is the accelerometer X acceleration channel
		case 5:
			//save sample
			acc_curX = ADC;

			//switch channel
			adc_chsel++;

			//setup adc mux to sample next channel and SC
			adc_mux();
			ADC_SC;
			break;


		//this is the accelerometer Y acceleration channel
		case 6:
			//save sample
			acc_curY = ADC;

			//switch channel
			adc_chsel++;

			//setup adc mux to sample next channel and SC
			adc_mux();
			ADC_SC;
			break;


		//this is the accelerometer Z acceleration channel
		case 7:
			//save sample
			acc_curZ = ADC;

			//use default case

		//reset the channel selector and stop int
		default:
			adc_chsel = 0;

			//setup adc mux to sample next channel
			adc_mux();
			break;
	}


}


//main initialization
void init()
{
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	//setup timer int for encoder sampling and system counter counting
	timer0_init();

	//initialize motor peripherals
	mot_init();

	//set up pwm
	pwm_init();

	//set up encoders
	enc_init();

	//setup adc circuit
	init_adc();

	//wait some time to ensure proper inititalization of rfm12 module after ISP usage
	_delay_ms(100);

	//enable rfm12
	rfm12_init();

	//init pid controllers
	pid_init();

	//enable interrupts
	sei();
}


//encoder delta and system counter interrupt handler
ISR(TIMER0_OVF_vect)
{
	//sample encoders
	enc_sample();

	//update system counter
	robo_sysCnt++;
}


//int td;
//debug string transmission
/*void radio_debug()
{
	char msg[255];

	//debug speed, pid output, pid istate, pid dstate,  speed measure time delta
	//sprintf(msg, "v:%i o:%i i:%i d:%i t:%i\r\n", robo_curSpeed, pid, robo_drivePID.iState, robo_drivePID.dState, td);

	//debug pid output, speed, target speed, set speed, movement mode
	//sprintf(msg, "o:%i v:%i t:%i s:%i m:%i\r\n", pid, robo_curSpeed, robo_targSpeed, move_setSpeed, move_mode);

	//debug adc - accelerometer
	sprintf(msg, "aX:%i aY:%i aZ:%i b:%i\r\n", acc_curX, acc_curY, acc_curZ, robo_battVolts);
	
	//debug adc - other, steering neutral position detector state
	//sprintf(msg, "c:%i b:%i n:%hi\r\n", robo_currentDrain, robo_battVolts, PINC & _BV(7));	

	rfm12_tx(strlen(msg), 0, (uint8_t *)msg);
}*/


//radio receive handler
void radio_rx()
{
	//process "valid" robo type packets, drop all others
	if((rfm12_rx_type() == RADIO_TYPE_ROBO) && (rfm12_rx_len() == 3))
	{
		switch(rfm12_rx_buffer()[0])
		{
			case RADIO_CMD_GOTO:
				move_encTarg = enc_A - rfm12_rx_buffer()[1];
				move_setSpeed = rfm12_rx_buffer()[2];
				move_mode = MOVE_MODE_GOTO;
				break;


			case RADIO_CMD_STOP:
			default:
				robo_targSpeed = 0;
				move_setSpeed = 0;
				move_mode = MOVE_MODE_STOP;
				break;
		}
	}
	
	//clear and switch receive buffer
	rfm12_rx_clear();
}


//radio communication
void radio_comm(void)
{
	radio_sensorData sensorData;
	
	//receive
	if(rfm12_rx_status() == STATUS_COMPLETE)
	{
		//handle reception
		radio_rx();
	}

	//transmit sensor data
	sensorData.acc_curX = acc_curX;
	sensorData.acc_curY = acc_curY;
	sensorData.acc_curZ = acc_curZ;
	sensorData.battVolts = robo_battVolts;
	sensorData.currentDrain = robo_currentDrain;
	sensorData.curSpeed = robo_curSpeed;
	sensorData.encAPos = enc_A;
	rfm12_tx(sizeof(sensorData), RADIO_TYPE_ROBO, (uint8_t *)&sensorData);
	
	//send debug information
	//radio_debug();

	//send alive message
	//rfm12_tx(sizeof(MSG_ALIVE), 0, MSG_ALIVE);
}


//calculate drive speed
//note: this function has to be called with a constant time delta
void calcSpeed()
{
	//last values
	static int32_t encLast;
	static uint32_t timeLast;	
	//speed history
	static int16_t speedHist[SPEEDHISTDEPTH]; //history of the last 4 speeds
	static uint8_t speedHistIdx; //index for speedHist	
	//temp values
	int16_t avgTmp = 0;
	int32_t encTmp;
	//index
	uint8_t i;

	//buffer value
	encTmp = enc_A;
	
	//calculate encoder A delta and divide by time since last measurement (current speed)
	//scale with SCHEDINT_MOVE
	speedHist[speedHistIdx++] = ((encTmp - encLast) * SCHEDINT_MOVE) / (int32_t)(robo_sysCnt - timeLast); 
	//wrap value (this is a circular buffer
	speedHistIdx %= SPEEDHISTDEPTH;
	
	//save last encoder position and time
	encLast = encTmp;	
	timeLast = robo_sysCnt;

	//average speed
	for(i = 0; i < SPEEDHISTDEPTH; i++)
	{
		avgTmp += speedHist[i];
	}

	robo_curSpeed = avgTmp / SPEEDHISTDEPTH;
}


//drive control
//this function controls the driving motor speed and sets the direction
//it is of vital importance that this function is called in fixed time intervals
void driveControl()
{
	//update current speed
	calcSpeed();

	//if we should move
	if(robo_targSpeed)
	{
		//pid control
		pid = MotorPID(&robo_drivePID, robo_targSpeed - robo_curSpeed, robo_curSpeed);

		//control driving motor direction and limit pwm value
		if(pid > 0)
		{
			mot1_fwd();
			robo_driveOut = min(pid, 255);
		}
		else //if(pid < 0)
		{
			mot1_rwd();
			robo_driveOut = min(-pid, 255);
		}

		//set motor pwm
		mot1_dty(robo_driveOut);
	}
	else
	{
		//turn on full motor brake
		mot1_brk();
		mot1_dty(0);

		//kill istate and dstate
		robo_drivePID.iState = 0;
		robo_drivePID.dState = 0;
	}
}

//control various movement states
void movementControl(void)
{
	//switch through movement modes
	switch(move_mode)
	{
		//in case we should reach a specific encoder position
		case MOVE_MODE_GOTO:
			//positive target reached
			if((robo_targSpeed > 0) && (enc_A >= move_encTarg))
			{
				//stop
				robo_targSpeed = 0;
				move_mode = MOVE_MODE_STOP;
			}
			//negative target reached
			else if((robo_targSpeed < 0) && (enc_A <= move_encTarg))
			{
				//stop
				robo_targSpeed = 0;
				move_mode = MOVE_MODE_STOP;
			}
			//target not reached
			else
			{
				//full throttle!
				robo_targSpeed = move_setSpeed;
			}
			break;


		//full throttle!
		case MOVE_MODE_FREE:
			robo_targSpeed = move_setSpeed;
			break;


		//stop
		case MOVE_MODE_STOP:
		default:
			robo_targSpeed = 0;
			break;
	}
}

//very simple back/forth driving behavior
void drive_test(void)
{
	if(enc_A > 64)
	{
		move_encTarg = -64;
		move_setSpeed = -20;
		move_mode = MOVE_MODE_GOTO;
	}
	else if(enc_A < -64)
	{
		move_encTarg = 64;
		move_setSpeed = 20;
		move_mode = MOVE_MODE_GOTO;
	}
}

//simple hackish joystick control function
void drive_joy(void)
{
	uint8_t joy;

	if(rfm12_rx_status() == STATUS_COMPLETE)
	{
		//if this is a joystick packet
		if((rfm12_rx_type() == 0x69) && (rfm12_rx_len() >= 1))
		{
			//-> F2 F1 RT LF DN UP
			joy = *(uint8_t *)rfm12_rx_buffer();

			//check vertical axis
			//initiate forward motion
			if(joy & 0x01)
			{
				move_setSpeed = 25;
				move_mode = MOVE_MODE_FREE;
			}
			//same backwards
			else if(joy & 0x02)
			{
				move_setSpeed = -25;
				move_mode = MOVE_MODE_FREE;
			}
			//stop motion
			else if((joy & 0x03) == 0)
			{
				move_setSpeed = 0;
				move_mode = MOVE_MODE_STOP;
			}

			//check horizontal axis
			//steer right
			if(joy & 0x04)
			{
				mot2_fwd();
			}
			//steer left
			else if(joy & 0x08)
			{
				mot2_rwd();
			}
			//straight
			else if((joy & 0x0C) == 0)
			{
				mot2_brk();
			}
		}
	}

	//clear buffer
	rfm12_rx_clear();
}


int main(void)
{
	uint32_t sched_radio, sched_drive, sched_move;

	//system initialization
	init();

	//debug wait (to better recognize atmel crashes)
	_delay_ms(500);

	//goto encoder pos
	//move_encTarg = 64;
	//move_setSpeed = 20;
	//move_mode = MOVE_MODE_GOTO;

	//motor 2 (steering) initialization
	mot2_brk();
	mot2_dty(255);

	//set accel +-2g scale
	DDRB |= _BV(3);
	PORTB &= ~_BV(3);
	
	//init lighting
	DDRD |= _BV(7);
	DDRC |= _BV(4);	
	PORTC |= _BV(4); //front/back lighting on
	PORTD |= _BV(7); //top lighting on
	
	//neutral position detector
	DDRC &= ~_BV(7);
	PORTC |= _BV(7);	

	//init scheduling points
	sched_radio = robo_sysCnt + 0x1000; sched_drive = robo_sysCnt; sched_move = robo_sysCnt + 0x0133;

	//main loop
	while (!0)
	{
		//drive control
		if(SCHED(sched_drive) > SCHEDINT_DRIVE)
		{
			//reset scheduling point
			sched_drive = robo_sysCnt;
			
			//call drive control
			driveControl();
		}


		//movement control
		if(SCHED(sched_move) > SCHEDINT_MOVE)
		{
			//reset scheduling point
			sched_move = robo_sysCnt;

			//call movement control
			movementControl();

			//call test behavior
			//drive_test();
		}

		//use joystick control
		//drive_joy();


		//radio communication
		if(SCHED(sched_radio) > SCHEDINT_RADIO)
		{
			//reset scheduling point
			sched_radio = robo_sysCnt;

			//call radio communication
			radio_comm();

			//kick in adc interrupt
			ADC_SC;
		}

		//rfm12 tick
		rfm12_tick();
    }

    return 0;
}
