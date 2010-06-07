	
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./uart.h"
#include "./util.h"
#include "./i2cmaster.h"

/*
	ATMEGA644
	PB0 v deflector UP
	PB1 v deflector DOWN
	PB5(MOSI)->SPI->laser
	PD2(INT0)<-hsync
	PD3(INT1)<-vsync
	
	PD4 h motor rpm input 
	PD5 shield output always GND
	PD6 debug output
	PD7 shield output always GND
	<>Motor X
	<>I2C->ATMEGA32
		->GLCD
		<- PS2 keyboard
		<>Motor Y

*/


#define __DEBUG 1

//hardware definitions
#define mirrordutycyle 30 //in percent
#define mirrorcount_h 4	//mirrors on h disc

#define deflect_v_TIME_ON 1	//mirrors on v disc
#define deflect_v_TIME_WAIT 1


//????
#define rpm_interval_v 20

volatile int fps=1;

//constant max. values
#define max_resolution_v 16
#define max_resolution_h 16

//not needed
#define maxpixel_h max_resolution_h	   

//current position of the laser point
volatile unsigned int cpixel_v=0;		
volatile unsigned int cpixel_h=0;

//output buffer
volatile unsigned char array[maxpixel_h/8];
//current test
volatile unsigned char test=0;		//current test running

//unit CPU cyles
volatile unsigned int laserwait_h = 2000;	//how long to wait from h_sync to start to display
volatile unsigned int laserwait_v = 100;	//how long to wait from v_sync to start to display

volatile unsigned int rpm_h=0;		//store the current rpm
volatile unsigned int rps_h_cnt=0;		//counts current rpm (always <= rmp_h)
volatile unsigned int rps_h =0;

volatile unsigned int rps_h_cnt_m=0; //rps from h-motor controller
volatile unsigned int rps_h_m=0;

volatile unsigned int rpm_v_cnt=0;
volatile unsigned int rpm_v=0;

volatile unsigned char timecounter=0;  //to get 1Hz sync

volatile unsigned char laserON=0; //laser diode status

volatile unsigned char deflectorStatus=0;

//reduce stretching
//precalculate values and store them here
//how many steps do we need ? (multiple of 2, <= maxpixel_h)
#define maxstretchsteps maxpixel_h/2

unsigned int h_stretch[maxstretchsteps];	//array for storring the precalculated values

float stretchstrength_h = 1;	// defines the degree of stretching the image
float stretchstrength_v = 1;

void hsync();
void vsync();
void hw_init();
void calc_h_stretch();
void soft_vsync();

void hw_init(void)
{

//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	//******************
	//ANALOG COMP 
	//******************
	//disable

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//******************
	//TIMER0 
	//******************
	//for h sync mode

	//Overflow value
	OCR0A = 0xFF;
	
	TCCR0A = 0; 		//no external waveform generation
	TCCR0B = _BV(CS00); 	//clock source, no prescaler
	//TIMSK0=(1<<OCIE0A); 	//timer0 OCR0A compare interrupt enable


	//******************
	//TIMER1 
	//******************
	//for soft v sync mode

	TCCR1A=0;		//no external waveform generation
	TCCR1B=0; 		//timer1 off
	//TIMSK1=(1<<OCIE1A); 	//Timer1 match Output Compare A Interrupt Enable


	//******************
	//TIMER2 
	//******************
	//1 Hz sync
	//timer2 interupt every 1/(F_CPU/1024*256) s

	TCCR2A=0;
	TCCR2B=(1<<CS22)|(1<<CS21)|(1<<CS20);		//F_CPU/1024 (From prescaler)
	TIMSK2=(1<<TOIE2); 	//Timer2 Overflow Interrupt Enable


	//******************
	// EXTERNAL INT0 
	//******************
	//interrupt on INT0 pin falling edge (sensor triggered) 

	EICRA |= _BV(ISC01);// | _BV(ISC00);
	
	//******************
	// EXTERNAL INT1
	//******************
	// interrupt on INT1 pin falling edge (sensor triggered) 
	//MCUCR |= (1<<ISC11) ;//| (1<<ISC10);

	//Allow interrupts on external pins
	EIMSK |= _BV(INT0);//|(1<<INT1); 
	

	//******************
	// SPI
	//******************
	//init SPI for laser
	//MOSI PIN output ?
	DDRB |= _BV(PB5);
	
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0); //SPI enable,master mode,speed at 1/128
	
	//DORD: LSB/MSB first ?
	//Double speed on
	//SPSR = (1<<SPI2X);
	
	//******************
	//PortB
	//******************
	DDRB=(1<<1)|(1<<2); //Pin 0&1 Output
	PORTB = (1<<1)|(1<<2);	//Pin 0&1 Pullup, other Pullups off
	
	//******************
	// PORTD DDR
	//******************
	
	DDRD = ~ (_BV(PD2) | _BV(PD3));	//Port D 2 & 3 input 
	PORTD |= _BV(PD3) | _BV(PD2) | _BV(PD4);	//Internal Pullups on pin 2&3&4
	
	//******************
	//Port A 
	//DDRA=0xFF;
	PORTA = 0x00;	//Internal Pullups all off
	
	//PORTD interrupt pin change
	PCICR=(1<<PCIE3);//enable pin change int on PD
	PCMSK3=(1<<PCINT28);//enable pin change int on PD4
	
}
	
int main (void)
{
  hw_init();
#ifdef __DEBUG
  uart_init();
  uart_putstr_P(PSTR("ATMEGA644 LASERPROJECTOR\n"));
  uart_putstr_P(PSTR("no console yet\nHELP\n1: laserwait_h\n2: laserwait_v\ns: Status\nw: stretchstrength_h\nh: print this text\n"));
#endif  
  calc_h_stretch();

  //start deflector_v
  initdeflector_v();

  char buffer[2];
  char RS232cmd=0;
  
   target_rpm_v=fps*max_resolution_v;
   sei();
   
   while(laserON == 0)//wait until h-motor is fast enough
     wait(100);

   PORTD |= _BV(PD6);//laser on	 
	 
   while (1) {

#ifdef __DEBUG
    if(uart_getc_nb(&RS232cmd)>0)//send req. via RS232 ?
      {
	if(RS232cmd == 'h'){
	  //help
	  uart_putstr_P(PSTR("HELP\n1: laserwait_h\n2: laserwait_v\ns: Status\nw: stretchstrength_h\nh: print this text\n"));
	}
	if(RS232cmd == '1'){
	  //help
	  uart_putstr_P(PSTR("laserwait_h = "));
	  laserwait_h=(unsigned int)(((unsigned char)uart_getc()<<8)|(unsigned char)uart_getc());
	  itoa(laserwait_h,buffer,10);
	  uart_putstr(&buffer[0]);
	}
	if(RS232cmd == '2'){
	  //help
	  uart_putstr_P(PSTR("laserwait_v = "));
	  laserwait_v=(unsigned int)(((unsigned char)uart_getc()<<8)|(unsigned char)uart_getc());
	  itoa(laserwait_v,buffer,10);
	  uart_putstr(&buffer[0]);
	}
	if(RS232cmd == 'w'){
	  //help
	  uart_putstr_P(PSTR("stretchstrength_h = "));
	  stretchstrength_h =(float)(((float)uart_getc())+(float)uart_getc()/100);
	  itoa(stretchstrength_h,buffer,10);
	  uart_putstr(&buffer[0]);
	}
	if(RS232cmd == 'a'){
	  //help
	  uart_putstr_P(PSTR("stretchstrength_v = "));
	  stretchstrength_v =(float)(((float)uart_getc())+(float)uart_getc()/100);
	  itoa(stretchstrength_v,buffer,10);
	  uart_putstr(&buffer[0]);
	}

	if(RS232cmd == 's'){
	  uart_putstr_P(PSTR("\ntest: "));
	  itoa(test,buffer,10);
	  uart_putstr(&buffer[0]);
	  
	  uart_putstr_P(PSTR("\nlaserwait_h: "));
	  itoa(laserwait_h,buffer,10);
	  uart_putstr(&buffer[0]);
	      
	  uart_putstr_P(PSTR("\nlaserwait_v: "));
	  itoa(laserwait_v,buffer,10);
	  uart_putstr(&buffer[0]);
	  	  
	  uart_putstr_P(PSTR("\nstretchstrength_h: "));
	  itoa( stretchstrength_h,buffer,10);
	  uart_putstr(&buffer[0]);
	  
	  uart_putstr_P(PSTR("\nstretchstrength_v: "));
	  itoa( stretchstrength_v,buffer,10);
	  uart_putstr(&buffer[0]);

	  
	}

	uart_putstr_P(PSTR("\n"));
	RS232cmd = 0;
      }    
#endif

    //LED blink
    PORTD &= ~ _BV(PD5);
    wait(200);
    PORTD |= _BV(PD5);
    wait(800);
    
    test++;
    soft_vsync();
    if (test > 7)
      test = 1;
  }
}

static void calc_h_stretch()
{
  unsigned char i=0;
  for (i = 0; i < maxstretchsteps; i++)
    h_stretch[i]=abs(i-(maxstretchsteps>>1))*stretchstrength_h;
}


//called by hardware
// -- hsync
ISR (SIG_INTERRUPT0)
{
  if(laserON == 0)
    return;

  //now wait until laser is in position
  //use timer0 interrupt

 rps_h_cnt++;// count current rpm_h
 if(TIMSK0 > 0)
   return;

  if(laserwait_h < 0xFF){
    TCCR0B=(1<<CS00); //clock source, no prescaler
    OCR0A=(unsigned char)(laserwait_h);
  }
  if(laserwait_h >= 0xFF && laserwait_h < 0xFD0){
    TCCR0B=(1<<CS01); //clock source 1/8 prescaler
    OCR0A=(unsigned char)((laserwait_h/0x08));
  } 
   if(laserwait_h >= 0xFD0 && laserwait_h < 0x10000){
    TCCR0B=(1<<CS02); //clock source 1/256 prescaler
    OCR0A=(unsigned char)((laserwait_h/0xFF));
  } 
  TIMSK0=(1<<OCIE0A); 	//timer0 OCR0A compare interrupt enable
}


//called by SIG_INTERRUPT0
// -- hsync  
ISR (TIMER0_COMPA_vect)
{
  TIMSK0=0;//disable timer0 interrupt
  TCCR0B=0;//disable timer0

  hsync();
}

static void hsync(){			//called by hardware, laser is in position now

	//cpixel_h=0;		//h pointer to 0

	//wait nops
	unsigned long waitloops=0;
	unsigned int waitfor=0;

	//now we got 8*8 ops
        //calculate how long to wait (horizontal)
        //in der zeit die der spiegel braucht max_resolution_h pixel anzeigen
        //zeit die der laser pro zeile proj.: mirrordutycyle/100 * 1/(rps_h*mirrorcount_h)
        //zeit zu warten: t1 = mirrordutycyle/(rps_h*mirrorcount_h*max_resolution_h*100)
        //(
	//cnt = t1/(1/F_CPU)
	//-----------------------------------
        //mirror moves faster on the edges
        //slower in the middle -> compensate
        //wait longer in the middle
        //( h_stretch[cpixel_h*maxstretchsteps/maxpixel_h] *mirrordutycyle/(rpm_h*mirrorcount_h*max_resolution_h*100)
	//
	waitfor=(mirrordutycyle*F_CPU)/(rps_h*mirrorcount_h*max_resolution_h*100);

#ifdef __DEBUG
	
	for(cpixel_h=0;cpixel_h < max_resolution_h;cpixel_h++){
	    
	  if ((array[(unsigned int)(cpixel_h/8)]&(1<<(cpixel_h%8))) != 0)
	    PORTD |= _BV(PD6);//turn on Pin6
	  else
	    PORTD &= ~ _BV(PD6);//turn of Pin6
	  
#else	
	//we draw 8 pixel at once
  	for(cpixel_h=0;cpixel_h < max_resolution_h;cpixel_h+=8){
		//draw stuff here


		/* Wait for SPI transmission complete */
		while(!(SPSR & (1<<SPIF)))
		  asm("nop");
		  

		//write 1 byte to SPI register
		//SPI sends it serialized to the laser
		/* Start transmission */
		SPDR = array[cpixel_h / 8];	//because we load 8 bit at once
		
#endif
		//now we got 8*8 ops
		//calculate how long to wait (horizontal)
		//in der zeit die der spiegel braucht max_resolution_h pixel anzeigen
		//zeit die der laser pro zeile proj.: mirrordutycyle/(rpm_h*mirrorcount_h*100)
		//zeit zu warten: mirrordutycyle/(rpm_h*mirrorcount_h*max_resolution_h*100)
		//-----------------------------------
		//mirror moves faster on the edges
		//slower in the middle -> compensate
		//wait longer in the middle
		//( h_stretch[cpixel_h*maxstretchsteps/maxpixel_h] *mirrordutycyle/(rpm_h*mirrorcount_h*max_resolution_h*100)
		
	    
		//später mit ISR ?
		//	for(waitloops=0;waitloops<h_stretch[(unsigned char)(cpixel_h*maxstretchsteps/maxpixel_h)]+waitfor;waitloops++)
		for(waitloops=0;waitloops<70;waitloops++)
		 asm("nop");
	}

	if(laserON != 0)
	  PORTD |= _BV(PD6); //laser on
}

static void soft_vsync(){		//generated by software after line change

        //cpixel_v++;		//next line

	//if(cpixel_v > max_resolution_v){
	  //TCCR1B=0;	//Timer1 Off
	  //TIMSK1=0;//Timer 1 interrupt  off
	  //cpixel_v=0;
        //}


	//now calculate next line

	int i=0;
	if(test == 1){
	  //test triangles
	  for(i=0;i < max_resolution_h/8;i++)
	    {
	      array[i]=(unsigned char) (1<<(cpixel_v&0x07));
	    }
	}

	if(test == 2){
	  //test horizontal lines
	  for(i=0;i < max_resolution_h/8;i++)
	    {
	      array[i]=(unsigned char) (1<<(cpixel_v&0x04));
	    }
	}

	if(test == 3){
	  //test 010101...
	  for(i=0;i < max_resolution_h/8;i++)
	    {
	      array[i]=(unsigned char) (0x55<<(cpixel_v&1));
	    }
	}
	
	if(test == 4){
	  //test full 
	  for(i=0;i < max_resolution_h/8;i++)
	    {
	      array[i]= (unsigned char)0xFF;
	    }

	}

	if(test == 5){
	//test first & last 8
    	  array[0]=(unsigned char) 0xFF;
	  array[max_resolution_h/8]=(unsigned char) 0xFF;
	}

	if(test == 6){
	//test vertical lines
	  for(i=0;i < max_resolution_h/8;i++)
	    {
	      array[i]=(unsigned char) 0x00;
	      if(i&8)
		array[i]=(unsigned char) 0xFF;
	    }
	}

	if(test == 7){
	//test first top, last bottom
	  if(cpixel_v == 0 || cpixel_v == max_resolution_v)
	    for(i=0;i < max_resolution_h/8;i++)
	      array[i]=(unsigned char) 0xFF;
	}
}


//called by hardware
//-- vsync
//TODO: change to soft
ISR (SIG_INTERRUPT1)
{
	//configure timer1 to call soft_vsync in def. time intervall
	TCCR1B=(1<<CS10);	//Timer1 On, no prescaler
	TIMSK1=(1<<OCIE1A);//Timer 1 Interrupt on
	//time to wait per rotation t=1/rpm_v
	//time per mirror t=1/(rpm_v*mirrorcount_v)
	//time per pixel_v t=1/(rpm_v*max_resolution_v*mirrorcount_v)
	//cycles to wait n = t / (1/F_CPU)= t * F_CPU
	//add coefficient stretchstrength_v

	OCR1B=F_CPU/(rpm_v*max_resolution_v*mirrorcount_v)*stretchstrength_v;

	//reset laser position
	cpixel_v=0;

	//count rpm_v
	rpm_v_cnt++;

	//wait nops
	unsigned int waitloops=0;
	//now wait until laser is in position
	for(waitloops=0;waitloops<laserwait_v;waitloops++);
}

//Timer1 called by SIG_INTERRUPT1
//Timer1 Output Compare A match interrupt
//needed for correct vertical display
ISR (TIMER1_COMPB_vect)
{
 TCNT1=0;
 deflector_handle();
}

//Timer2 Overflow interrupt
//time reference
//called ~100x per second
ISR (TIMER2_OVF_vect)
{
  char buffer[2];
  timecounter++;
  if( timecounter > (F_CPU/(0x40000)) ){	//execute once a second
    rps_h=(unsigned int)(rps_h_cnt/mirrorcount_h);
    rps_h_m =(unsigned int)(rps_h_cnt_m / 12);
    rpm_h=rps_h*60;
    rpm_v=rpm_v_cnt;
    rps_h_cnt = 0;
    rpm_v_cnt = 0;
    rps_h_cnt_m=0;
    timecounter = 0;
    uart_putstr_P(PSTR("\nrps_h: "));
    itoa(rps_h,buffer,10);
    uart_putstr(&buffer[0]);

    //test if h_motor is fast enough
    if(rps_h_m > 30)
      laserON=1;
    else
      laserON=0;
           
  }
}

 ISR(PCINT3_vect)
 {
   rps_h_cnt_m++;
 }

 //configure timer1 for soft_vsync
 static void initdeflector_v(){
   //enable deflector_v
   PORTD = 1;
   
   //configure timer1 to call soft_vsync in def. time intervall
   TCCR1B=(1<<CS10)|(1<<CS12);	//Timer1 On, prescaler 1/1024
   TIMSK1=(1<<OCIE1A);//Timer 1 Interrupt on

   OCR1B=deflect_v_TIME_ON/2;

   //count rpm_v
   rpm_v_cnt++;

   deflectorStatus=1; //ON up
 }


 //configure timer1 for soft_vsync
 static void deflector_v_off(){
   //enable deflector_v
   PORTD = 0;
   
   //configure timer1 to call soft_vsync in def. time intervall
   TCCR1B=0;//Timer1 Off, prescaler 1/1024
   TIMSK1=0;//Timer 1 Interrupt off

   deflectorStatus=0; //OFF
 }

 //configure timer1 for soft_vsync
 static void deflector_handle(){
   //enable deflector_v

   if(deflectorStatus == 1){
    PORTD=0;
    //count rpm_v
    rpm_v_cnt++;
    OCR1B=deflect_v_TIME_WAIT;
   }
   if(deflectorStatus == 2){
    PORTD=2;
    OCR1B=deflect_v_TIME_ON;
   }
   if(deflectorStatus == 3){
    PORTD=0;
    OCR1B=deflect_v_TIME_WAIT;
   }
   if(deflectorStatus == 4){
    PORTD=1;
    OCR1B=deflect_v_TIME_ON;
    deflectorStatus = 0;
   }

   deflectorStatus++; //next step
 }


void i2controlstuff(){
  unsigned char ret=0;
  //DevAddr = 1 ?
  unsigned char DevAddr=0;
   ret = i2c_start(DevAddr+I2C_WRITE);       // set device address and write mode
    if ( ret ) {
        /* failed to issue start condition, possibly no device found */
        i2c_stop();
    }else {
        /* issuing start condition ok, device accessible */
        i2c_write(0x01);                       // write 0x01, request key pressed on keyboard
        //i2c_write(0x75);                       // ret=0 -> Ok, ret=1 -> no ACK 
        //i2c_stop();                            // set stop conditon = release bus
    
	i2c_rep_start(DevAddr+I2C_READ);       // set device address and read mode
        ret = i2c_readNak();                    // read one byte
        i2c_stop();
        //do stuff
	//fake RS232 event
	//RS232cmd=ret;
	
	//i2c_start_wait(DevAddr+I2C_WRITE);     // set device address and write mode
        //i2c_write(0x02);                        // write command
        //i2c_write(0x01);//set target rpm_v
	//i2c_write((unsigned char)(target_rpm_v)>>8);
	//i2c_write((unsigned char)(target_rpm_v&0xFF));

	//i2c_rep_start(DevAddr+I2C_READ);        // set device address and read mode
        //ret = i2c_readAck();                       // read one byte form address 0
        //ret = i2c_readAck();                       //  "    "    "    "     "    1
        //ret = i2c_readAck();                       //  "    "    "    "     "    2
        //ret = i2c_readNak();                       //  "    "    "    "     "    3
        //i2c_stop();           
    }
}

