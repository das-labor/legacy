/*############################################################################
############################################################################*/

#include "main.h"

unsigned char twi_state = 0;
unsigned char motor = 0;
unsigned char motorread = 0;
unsigned char motor_rx[8];

//############################################################################
//Initzialisieren der I2C (TWI) Schnittstelle
void i2c_init(void)
//############################################################################
{
  TWSR = 0;
  TWBR = ((SYSCLK/SCL_CLOCK)-16)/2; 
}

//############################################################################
//Start I2C
char i2c_start(void) 
//############################################################################
{
    TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT) | (1<<TWIE);
    return(0);
}

//############################################################################
//Start I2C
void i2c_stop(void)
//############################################################################
{
    TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);
}

void i2c_reset(void)
//############################################################################
{
                 i2c_stop();                
                 twi_state = 0;
                 motor = TWDR;
                 motor = 0;
                 TWCR = 0x80;
                 TWAMR = 0;
                 TWAR = 0;
                 TWDR = 0;
                 TWSR = 0;
                 TWBR = 0;
                 i2c_init();
                 i2c_start();
                 i2c_write_byte(0);
}

//############################################################################
//Start I2C
char i2c_write_byte(char byte)
//############################################################################
{ 
    TWSR = 0x00;
    TWDR = byte;
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
    
    return(0);
    
}

//############################################################################
//Start I2C
SIGNAL (TWI_vect)
//############################################################################
{
    switch (twi_state++)
        {
        case 0:
                i2c_write_byte(0x52+(motor*2));
                break;
        case 1:
                switch(motor++)
                    {
                    case 0:
                            i2c_write_byte(Motor_Vorne);
                            break;
                    case 1:       
                            i2c_write_byte(Motor_Hinten);
                            break;
                    case 2:
                            i2c_write_byte(Motor_Rechts);
                            break;
                    case 3:
                            i2c_write_byte(Motor_Links);
                            break;
                    }
                break;
        case 2:
                i2c_stop();
                if (motor<4) twi_state = 0;
                else motor = 0;
                i2c_start();   
                break; 
                    
        //Liest Daten von Motor
        case 3:
                i2c_write_byte(0x53+(motorread*2));
                break;
        case 4:
                switch(motorread)
                    {
                    case 0:
                        i2c_write_byte(Motor_Vorne);
                        break;
                    case 1:
                        i2c_write_byte(Motor_Hinten);
                        break;
                    case 2:
                        i2c_write_byte(Motor_Rechts);
                        break;
                    case 3:
                        i2c_write_byte(Motor_Links);
                        break;
                    }
                break;
        case 5: //1 Byte vom Motor lesen       
                motor_rx[motorread] = TWDR;

        case 6:
                switch(motorread)
                    {
                    case 0:
                        i2c_write_byte(Motor_Vorne);
                        break;
                    case 1:
                        i2c_write_byte(Motor_Hinten);
                        break;
                    case 2:
                        i2c_write_byte(Motor_Rechts);
                        break;
                    case 3:
                        i2c_write_byte(Motor_Links);
                        break;
                    }
                break;  
        case 7: //2 Byte vom Motor lesen       
                motor_rx[motorread+4] = TWDR;
                motorread++;
                if (motorread>3) motorread=0;
                i2c_stop();
                I2CTimeout = 10;
                twi_state = 0;
        }
 TWCR |= 0x80;
}
