//#include <util/delay.h>
#include "spi.h"
#include "can.h"
#include "twi_master.h"
#include "i2ctempsens.h"
#include "kuechenCAN.h"

#define R_LED _BV(PC1)
#define G_LED _BV(PC2)
#define B_LED _BV(PC3)


/*
	XXX Powercommander.h
*/
#define C_SW  (0x00)
#define C_PWM (0x01)
#define O_SW00 (0x00)
#define O_PWM05 (0x05)
#define F_SW_TOGGLE   (0x03)
#define F_PWM_MOD     (0x02)
#define F_PWM_DIR     (0x03)

#define PWM_KUECHE		(O_PWM05)
#define SWL_KUECHE		(O_SW00)

//0x02 0x05 0x02 0x00
//0x02 0x00 0x03 0x00
uint8_t light=0;
uint8_t lastlight=0;
uint8_t rgbled_stat=0;
uint8_t rgb_panic=0;
uint8_t lastpanic=0;
uint8_t panic=0;

// ALARM in hex
can_message panic_msg = {0x23, 0x00, ALARMCANPORT, 0x00, 0x06, {0x41,0x4C,0x41,0x52,0x4D,0x00}};
//can_message light_on_msg = {0x23, 0x02, LIGHTCANPORT, 0x01, 0x04, {0x00,0x00,0x01,0x00}};
//can_message light_off_msg = {0x23, 0x02, LIGHTCANPORT, 0x01, 0x04, {0x00,0x00,0x00,0x00}};

/*
	set as backgroundcolor
*/

i2c_message commblock = {0,0,0,{0,0,0,0,0,0,0,0}};
void twi_mhandler_read(i2c_message *indata)
{
	uint8_t i=0;
	can_message dstrl = {0x23,0x00,I2CTEMPCANPORT,I2CTEMPCANPORT,0x08, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	dstrl.dlc=indata->dlc+1;
	dstrl.data[0]=indata->process;
	for(i=0;i<indata->dlc;i++)
		{
			dstrl.data[i+1]=indata->data[i];
		}
	
	can_transmit(&dstrl);
}
// postwrite
void twi_mhandler_write(i2c_message *outdata)
{
	// not needed it is postwrite
}
void twi_mhandler_error(uint8_t error,i2c_message *errdata)
{
	//can_message dstrl = {0x23,0x00,I2CTEMPCANPORT,I2CTEMPCANPORT,0x06, {error,errdata->data[0],errdata->data[1],errdata->data[2],0x11,0x11}};
	//can_transmit(&dstrl);
  
}

void init_sensor()
{
	if(TWIM_Init()==0)
		{
			can_message dstrl = {0x23,0x00,I2CTEMPCANPORT,I2CTEMPCANPORT,0x06, {0x33,0x33,0x33,0x33,0x33,0x33}};
			can_transmit(&dstrl);
			
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=SOFTWARE_POR;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}
	
	commblock.addr_dst = 0x9e;
	commblock.dlc = 2;
	commblock.data[0]=ACCESS_CONFIG;
	commblock.data[1]=I2CDEFAULTCONFIG; 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=START_CONVERT;
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

}
/* data: 0x9e 0x01 0x02 0x22 0x8c 0x00  */
/* data: 0x9e 0x01 0x02 0xaa 0x8c 0x00  */
/* data: 0x8c 0x00 0x01 0x00 0x00 0x00  */
/* data: 0x9e 0x01 0x02 0x51 0x00 0x00  */
void read_sensor()
{

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=STOP_CONVERT;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=READ_TEMPERATURE;
	// set command last 
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 2;
	commblock.process=PROCESSI2CREAD;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

	commblock.addr_dst = 0x9e;
	commblock.dlc = 1;
	commblock.data[0]=START_CONVERT;
	commblock.process=PROCESSI2CWRITE;

	while(commblock.process != PROCESSI2CNONE)
		{
			taskDelayFromNow(10);
		}

}

/*
  dies funktion wird aufgerufen wenn das Packet
  an unser devid ging, wir uns also dafuer
  interressieren
*/
void can_user_cmd(can_message *rx_msg)
{
	if(rx_msg->port_dst == I2CTEMPCANPORT)
		read_sensor();
}


void app_run(void)
{ 
	// lampe
	clicked_0=0;
	held_0=0;
	if(!(PINB & _BV(PB1))) 
	{
		counter_0++;
		if(counter_0 > HOLD_THRESHOLD)
		{
			held_0 = 1;
			counter_0 = HOLD_THRESHOLD;
		}
	} 
	else
	{
		if(counter_0 > CLICK_THRESHOLD)
		{
			if(counter_0 < HOLD_THRESHOLD)
			{
				clicked_0 = 1;
			}
		}
		counter_0 = 0;
	}
	
	if(clicked_0 == 1)
	{
		dstpower.data[0] = C_SW;
		dstpower.data[1] = SWL_KUECHE;
		dstpower.data[2] = F_SW_TOGGLE;
		if(rgbled_stat==R_LED)
		{
			rgbled_stat=G_LED;
		}
		else
		{
			rgbled_stat=R_LED;
		}
		can_transmit(&dstpower);
	}

	if(held_0)
	{
		dstpower.data[0] = C_PWM;
		dstpower.data[1] = PWM_KUECHE;
		dstpower.data[2] = F_PWM_MOD;
		can_transmit(&dstpower);
	}
	else if(last_held_0)
	{
		dstpower.data[0] = C_PWM;
		dstpower.data[1] = PWM_KUECHE;
		dstpower.data[2] = F_PWM_DIR;
		can_transmit(&dstpower);
	}

	last_held_0 = held_0;

	//panic
	if((PINC & _BV(PC0)) && (panic==0))
	{
		panic=1;
	}
	if(!(PINC & _BV(PC0)) && (panic==1))
	{
		panic=2;
	}
	if(panic==2)
	{
		can_transmit(&panic_msg);
		if(lastpanic == 0)
			{
				rgb_panic=1;
				lastpanic=1;
			}
		// you can send panic as offen as you want
		panic=0;
	}
	
	_delay_ms(100);
}

#if 0
void appLoop_alarmt(void)
{ 
	while(true)
		{
			if(lastpanic==1)
				{
					// 10 sec blinken
					taskDelayFromNow(10000);
					rgb_panic=0;
					lastpanic=0;
				}
			taskDelayFromNow(100);
			
		}

}

void appLoop_alarm(void)
{ 
	while(true)
		{
			if(lastpanic==1)
				{
					if(rgb_panic == 1)
						{
							rgb_panic=2;
						}
					taskDelayFromNow(500);
					if(rgb_panic == 2)
						{
							rgb_panic=1;
						}
				}
			taskDelayFromNow(100);
		}

}
#endif


void update_leds(void)
{ 
	if((rgbled_stat & R_LED) !=0)
	{
		PORTC |= R_LED;
	}
	else
	{
		PORTC &= ~R_LED;
	}
	if((rgbled_stat & G_LED) !=0)
	{
		PORTC |= G_LED;
	}
	else
	{
		PORTC &= ~G_LED;
	}
	if( (rgbled_stat & B_LED) !=0)
	{
		PORTC |= B_LED;
	}
	else
	{
		PORTC &= ~B_LED;
	}

	if( rgb_panic == 1 )
	{
		PORTD |= _BV(PD5);
		PORTD |= _BV(PD6);
		PORTD |= _BV(PD7);
	}
	if( rgb_panic == 2 )
	{
		PORTD &= ~_BV(PD5);
		PORTD &= ~_BV(PD6);
		PORTD &= ~_BV(PD7);
	}
}


void io_init()
{
	rgbled_stat=R_LED;
	
	//Kuechenlicht
	DDRC |=  R_LED | G_LED | B_LED; // output led taster
	
	// led power on!
	PORTC &= ~(G_LED | R_LED);
	PORTC |= B_LED;
	// button
	DDRB &= ~_BV(PB1);      // in
	PORTB |= _BV(PB1);      // pullup on

	// XXX PC5 and PC4 are i2c for temp sense

	// Alarmbutton
	DDRD |= _BV(PD5) | _BV(PD6) | _BV(PD7);         // output
	
	// led power on!
	PORTD &= ~(_BV(PD5) | _BV(PD6) | _BV(PD7));     // off

	//Button
	DDRC &= ~_BV(PC0);      // in
	PORTC |= _BV(PC0);      // pullup on
}


void init(void)
{ 
	spi_init();
	can_init();
	TWIM_Init();
	init_sensor();
	io_init();
}


void main()
{
	can_message dstpower = {0x23,0x02,LIGHTCANPORT,0x01,0x04, {0x00,0x00,0x00,0x00}};ß
	uint8_t counter_0;
	uint8_t clicked_0 = 0;
	uint8_t held_0    = 0;
	uint8_t last_held_0;
	
	init();
	sei();
	
	while(true)
	{
		update_leds
		can_handler();
		twi_mhandler(); 
		app_run();
	}
}


