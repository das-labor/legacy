#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "lib-host/can.h"
#include "lib/lap.h"
#include "lib-host/debug.h"

#include "cmds-powermeter.h"

#define POWERMETER_ADDR 0x05
#define POWERMETER_PORT 0x3C


typedef struct
{
	int32_t Ueff;
	int32_t Ieff;
	int32_t P;
	int32_t S;
} powermeter_data_t;

typedef struct
{
	powermeter_data_t c1;
	powermeter_data_t c2;
	powermeter_data_t c3;

} powermeter_channel_t;


void cmd_canpowermeter(int argc, char *argv[])
{
	//unsigned char addr;
	can_message *msg;
	msg = can_buffer_get();
	can_message *result;
	powermeter_channel_t powermeter_powerdrawLastSecond;
	powermeter_data_t powermeter_sum;
	uint8_t buf[13];
	uint8_t end=0;
	uint8_t i;	//for counter
	for(i = 1;i < 14;i++)
		buf[i] = 0;

//	unsigned int mode = 0;

//	unsigned int scanned = 0;

//	int temperatur = 0;
//	unsigned char temp_msb = 0;
//	unsigned char temp_lsb = 0;
#if 0
	if (argc < 2 || argc > 3)
		goto argerror;
	scanned = sscanf(argv[1], "%x", (unsigned int*)&addr);
	if (argc == 3)
		scanned += sscanf(argv[2], "%x", (unsigned int*)&mode);

	msg->addr_src = 0x00;
	msg->addr_dst = addr;
	msg->port_src = 0x00;
	msg->port_dst = 0x10;
	msg->dlc      = 1;
	msg->data[0]  = 0;


	can_transmit(msg);
#endif
	for (;;)
	{
		result = can_get();

		if ((result != NULL) && result->addr_src == POWERMETER_ADDR && result->addr_dst == 0x00)
		{
			if(result->port_dst == POWERMETER_PORT)
			{
			if(result->data[0] > 0 && result->data[0] < 14)
				buf[result->data[0]]=1;

			switch(result->data[0])
			{
			case 0:
				//nothing to do here
				break;
			case 0x01:
				memcpy(&powermeter_powerdrawLastSecond.c1.P,&result->data[1],4);
				break;
			case 0x02:
				memcpy(&powermeter_powerdrawLastSecond.c2.P,&result->data[1],4);
				break;
			case 0x03:
				memcpy(&powermeter_powerdrawLastSecond.c3.P,&result->data[1],4);
				break;
			case 0x04:
				memcpy(&powermeter_powerdrawLastSecond.c1.S,&result->data[1],4);
				break;
			case 0x05:
				memcpy(&powermeter_powerdrawLastSecond.c2.S,&result->data[1],4);
				break;
			case 0x06:
				memcpy(&powermeter_powerdrawLastSecond.c3.S,&result->data[1],4);
				break;
			case 0x07:
				memcpy(&powermeter_powerdrawLastSecond.c1.Ueff,&result->data[1],4);
				break;
			case 0x08:
				memcpy(&powermeter_powerdrawLastSecond.c2.Ueff,&result->data[1],4);
				break;
			case 0x09:
				memcpy(&powermeter_powerdrawLastSecond.c3.Ueff,&result->data[1],4);
				break;
			case 0x0a:
				memcpy(&powermeter_powerdrawLastSecond.c1.Ieff,&result->data[1],4);
				break;
			case 0x0b:
				memcpy(&powermeter_powerdrawLastSecond.c2.Ieff,&result->data[1],4);
				break;
			case 0x0c:
				memcpy(&powermeter_powerdrawLastSecond.c3.Ieff,&result->data[1],4);
				break;
			}
			}
		}

		end = 0;
		for(i = 1 ; i < 14; i++)
			end += buf[i];
		if(end == 12)
			break;
	}
	powermeter_sum.P = powermeter_powerdrawLastSecond.c1.P + powermeter_powerdrawLastSecond.c2.P + powermeter_powerdrawLastSecond.c3.P;
	powermeter_sum.S = powermeter_powerdrawLastSecond.c1.S + powermeter_powerdrawLastSecond.c2.S + powermeter_powerdrawLastSecond.c3.S;
	powermeter_sum.Ueff = (powermeter_powerdrawLastSecond.c2.Ueff + powermeter_powerdrawLastSecond.c2.Ueff + powermeter_powerdrawLastSecond.c3.Ueff) /3;
	powermeter_sum.Ieff =  (powermeter_powerdrawLastSecond.c1.Ieff + powermeter_powerdrawLastSecond.c2.Ieff + powermeter_powerdrawLastSecond.c3.Ieff);

	printf("P is %4.2f\n", ((float)powermeter_sum.P * 0.00256299972534f) );
	printf("S is %4.2f\n",  ((float)powermeter_sum.S * 0.00256299972534f ));
	printf("Ueff is %3.2f\n", ((float)powermeter_sum.Ueff * 0.173217773437f));
	printf("Ieff is %2.3f\n", ((float)powermeter_sum.Ieff * 0.014796401515151f));

	return;
argerror:
	debug(0, "cantemp <addr> [mode]");
}

