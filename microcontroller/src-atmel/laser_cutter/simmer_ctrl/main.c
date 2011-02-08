
#include "uart/uart.h"
#include "../lib/com/com.h"

#define CS_WAIT_SYNC  0x00
#define CS_CMD1       0x01
#define CS_CMD2       0x02
#define CS_SLAVE1_REQ 0x03


uint8_t state_main_on;

uint8_t command_auto;

uint8_t command_zuenden;
uint8_t state_zuenden;

uint8_t command_500V_psu;
uint8_t state_500V_psu;

uint8_t command_simmer_psu;
uint8_t state_simmer_psu;


/*
//Command byte 1:
#define MSK_MAIN_CMD       0x01
#define MSK_AUTO_CMD       0x02
#define MSK_500V_PSU_CMD   0x08
#define MSK_ZUEND_CMD      0x10
#define MSK_SIMMER_CMD     0x20

//Command byte 2:
#define MSK_PUMPE_CMD      0x01
#define MSK_POWER_CMD      0x02
#define MSK_BRUECKEN_CMD   0x04
#define MSK_DUMP_CMD       0x08
#define MSK_FIRE_CMD       0x10

//Simmer Slave response byte
#define MSK_500V_PSU_STATE 0x08
#define MSK_ZUEND_STATE    0x10
#define MSK_SIMMER_STATE   0x20
*/

void slave_com(){
  uint8_t c, res;
  static uint8_t cmd1, cmd2;
  res = uart_getc_nb(&c);
  if(res == 1){
    switch(com_state){
      case CS_WAIT_SYNC:
        if(c == 0x42){
          com_state = CS_CMD1;
        }
        break;
      case CS_CMD1:
        cmd1 = c;
        com_state = CS_CMD2;
        break;
      case CS_CMD2:
        cmd2 = c;
        state_main_on = 
        com_state = CS_SLAVE1_REQ;
        break;
      case CS_SLAVE1_REQ:
        break;
        
      
    
    }
  
  
  }

}



int main(){
  uart_init();
  uart_putstr("Hellow World\r\n");
}