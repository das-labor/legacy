
#include <avr/io.h>
#include "i2csw.h"

// This Library uses slow (100kHz) Bus timing


inline void delay_5u(){
	asm volatile(
	"delay_5u_lp%=:\n\t"
		"dec	%0	\n\t"
		"brne	delay_5u_lp%=\n\t"
		::"r" ((u08)((5ul*F_CPU)/(3ul*1000000ul))) ); 
}

inline void delay_2u5(){
	asm volatile(
	"delay_2u5_lp%=:\n\t"
		"dec	%0	\n\t"
		"brne	delay_2u5_lp%=	\n\t"
		::"r" ((u08)((2.5*F_CPU)/(3ul*1000000ul))) ); 
}


#define QDEL() delay_2u5()
#define HDEL() delay_5u()

//Control I2C bus with DDR register to simulate open collector behavior
#define SDA_LO()      SDADDR |=  (1<<SDA)
#define SDA_HI()      SDADDR &= ~(1<<SDA)

#define SCL_LO()      SCLDDR |=  (1<<SCL) 
#define SCL_HI()      SCLDDR &= ~(1<<SCL) 


// defines and constants
#define READ		0x01	// I2C READ bit


//generate a start or repeated start condition
void i2cStart(){
	QDEL();
	SDA_HI();
	HDEL();
	SCL_HI();
	HDEL();
	SDA_LO();
	HDEL();
	SCL_LO();
}

void i2cStop(){
	HDEL();
	SCL_HI();
	HDEL();
	SDA_HI();
}

u08 i2cPutbyte(u08 b)
{
	u08 i, ack;
	
	for (i=0;i<8;i++){
		QDEL();
		if ( b & 0x80 ){
			SDA_HI();
		}else{
			SDA_LO();
		}
		
		QDEL();
		SCL_HI();	//clock out bit
		HDEL();
		SCL_LO();
		b <<= 1;
	}

	QDEL();
	SDA_HI();		//release SDA to be able to get ack
	QDEL();
	SCL_HI();		//clock in ack
  	
	QDEL();
	ack = SDAPIN & (1<<SDA);	// get the ACK bit
	QDEL();

	SCL_LO();
	QDEL();
	SDA_LO();					//leave with SDA low
	return (ack == 0);			// return ACK value
}


u08 i2cGetbyte(u08 ack)
{
	u08 i, b = 0;
    
	QDEL();
	SDA_HI();
    
	for(i=0;i<8;i++){
		HDEL();
		b <<= 1;
		SCL_HI();				//clock in bit
	  	HDEL();
		if(SDAPIN & (1<<SDA)) b |= 1;//read bit
		SCL_LO();
	}
  
	QDEL();

	//only ack if requested
	if (ack){
		SDA_LO();
	}
	
	QDEL();
	SCL_HI();
	HDEL();
	SCL_LO();

	return b;					// return received byte
}

//this is a dummy for now
void i2cInit(void)
{
}

//write to i2c eeprom in multibyte write mode
s08 i2cEeWrite(u16 address, u16 len, u08 *data)
{
	u08 ack;
	union{u08 b[2]; u16 w;} addr;
	addr.w = address;
	
	while(len){
		i2cStart();
		//set start address
		if (! i2cPutbyte(EEPROM_ADDRESS|(addr.b[1]<<1))) goto error;
		if (! i2cPutbyte(addr.b[0])) goto error;

		do{
			len--;
			if(! i2cPutbyte(*data++) ) goto error;
			addr.w++;
		//keep writing until a 4 byte boundary in eeprom is reached
		}while(len && (addr.b[0] % 4));
	
		//initiate and wait for internal write cycle
		do{
			i2cStop();
			i2cStart();
			ack = i2cPutbyte(EEPROM_ADDRESS);
		}while(!ack);
	
		i2cPutbyte(0);
		i2cStop();		
	}
	
	return 0;
	
error:
	i2cStop();
	return -1;
}

//read bytes from eeprom to buffer
void i2cEeRead(u16 address, u16 len, u08 *data)
{
	u08 ack;
	union{u08 b[2]; u16 w;} addr;
	addr.w = address;
	
	while(len){
		i2cStart();
		//setup address
		i2cPutbyte(EEPROM_ADDRESS | (addr.b[1]<<1) );
		i2cPutbyte(addr.b[0]);
	
		//do a repeated start condition
		i2cStart();
	
		i2cPutbyte(EEPROM_ADDRESS | READ | (addr.b[1]<<1) );

		do{
			//keep on going until all bytes are sent, or the end of a field is reached
			ack = (len != 1) && (addr.b[0] != 0xff); 
			*data++ = i2cGetbyte(ack);//send no ack on last byte
			addr.w++;
			len--;
		}while (ack);
		
		i2cStop();
	}
}


//Probes if a device with this address acks
u08 i2cProbe(u08 addr){
	u08 ack;
	i2cStart();
	ack = i2cPutbyte(addr);
	i2cStop();
	return ack;
}

//detect if there is a valid i2c eeprom card inserted
//returns card size in sectors of 256 bytes
//a return value of zero means no valid card
u08 i2cEeDetect(){
	u08 retval;
	HDEL();
	if( !(SDAPIN & (1<<SDA)) ){
		//SDA is low, so this can't be an i2c card
		return 0;		
	}
	
	if(i2cProbe(0x23)){
		//device 0x23 shouldn't be present, so this is not a valid card
		return 0;		
	}
	
	//find out size of card by probing sub addresses - max 2kiB
	for(retval=0;retval<8;retval++){
		if( ! i2cProbe(0xa0|(retval<<1)) )
			break;			
	}
	
	if(i2cProbe(0x67)){
		//device 0x67 shouldn't be present, so this is not a valid card
		return 0;	
	}
	
	//seems to be a valid card, so we return the size
	return retval;
}
