
#include "ICcom.h"
#include "uart.h"
#include <stdio.h>

#define ICC_PACKET1 8			//max instructions here

void * icc_var_array[ICC_MAX_VARS];

static uint8_t icc_status=0;
static uint8_t icc_packetleng=0;

void icc_handle_uart(void){
	uint8_t c=0;
	
	if(ICC_WAITFORPACKET)
	{
		c=uart_getc();		//get first byte from queue
		if((c & 0xF8) != MAGIC_PACKET)
			return;			//didn't find magic bits

		icc_packetleng = (c & 0x7);
		if(icc_packetleng == 0)
			return;			//packet length == 0, skip this one

		ICC_SETSTATUSRECPACKET;
	}
	else
	{
		if(uart_getrxqueuelength() < icc_packetleng)		//check if packet is received
			return;

		ICC_SETSTATUSWAITPACKET; //set status to: wating for next packet

		uint8_t ID=uart_getc();
		
		if(ID && (ID < ICC_MAX_VARS)){
			icc_packetleng--;
			
			
			if(!ID)
				return;
			
			if(icc_packetleng == 1)
				*(uint8_t *)icc_var_array[ID] = uart_getc();
			if(icc_packetleng == 2)
			{
				*((uint8_t *)icc_var_array[ID]+1) = uart_getc();
				*(uint8_t *)icc_var_array[ID] = uart_getc();
			}
			if(icc_packetleng ==4)
			{
				*((uint8_t *)icc_var_array[ID]+3) = uart_getc();
				*((uint8_t *)icc_var_array[ID]+2) = uart_getc();
				*((uint8_t *)icc_var_array[ID]+1) = uart_getc();
				*(uint8_t *)icc_var_array[ID] = uart_getc();
			}
		}else if(ID == 0){	//echo ?

		}
	
	}

}

static uint8_t ICC_getID(void *var)
{
	if(var == NULL)
		return 0;	//not a valid pointer
	
	uint8_t i=0;
	for(i=0; i<ICC_MAX_VARS;i++)
	    if(*(uint8_t *)var == *(uint8_t *)icc_var_array[i])
			return i;
	return 0;	//return 0 if not found
}

static uint8_t ICC_send_uint8(uint8_t *c)
{
	uint8_t ID = ICC_getID(c);
	if(ID){
		uart_putc(MAGIC_PACKET|2);
		uart_putc(ID);
		uart_putc(*c);
	}
	return ID;
}
static uint8_t ICC_send_uint16(uint16_t *c)
{
	uint8_t ID = ICC_getID(c);
	if(ID){
		uart_putc(MAGIC_PACKET|3);
		uart_putc(ID);
		uart_putc((uint8_t)(*c>>8));
		uart_putc((uint8_t)(*c&0xFF));
		}
	return ID;
}
static uint8_t ICC_send_uint32(uint32_t *c)
{
	uint8_t ID = ICC_getID(c);
	if(ID){
		uart_putc(MAGIC_PACKET|5l);
		uart_putc(ID);
		uart_putc((uint8_t)(*c>>24));
		uart_putc((uint8_t)(*c>>16));
		uart_putc((uint8_t)(*c>>8));
		uart_putc((uint8_t)(*c&0xFF));
		}
	return ID;
}
