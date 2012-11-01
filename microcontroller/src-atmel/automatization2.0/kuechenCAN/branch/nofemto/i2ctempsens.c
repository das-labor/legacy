#include "twi_master.h"

// user has to define commblock
i2c_message commblock = {0,0,0,{0,0,0,0,0,0,0,0}};

void twi_mhandler()
{
	Tuint08 res=EI2CNONE;
	if( (commblock.process & PROCESSI2CREAD) == PROCESSI2CREAD )
	{
		if (!TWIM_Start (commblock.addr_dst, TW_READ))
		{
			TWIM_Stop (); 
			res +=EI2CREAD;
			// comm failed
		}
		else
		{
			Tuint08 i=0;
			// last byte via nack
			for(;i<((commblock.dlc)-1);i++)
			{
				commblock.data[i] = TWIM_ReadAck ();
			}
			commblock.data[i] = TWIM_ReadNack ();
			TWIM_Stop();
			twi_mhandler_read(&commblock);
			commblock.process -= PROCESSI2CREAD;
			// comm ok
		}
	}
	if( (commblock.process & PROCESSI2CWRITE) == PROCESSI2CWRITE )
	{
		if (!TWIM_Start (commblock.addr_dst, TW_WRITE))
		{
			TWIM_Stop ();
			res += EI2CWRITE;
		}
		else
		{
			Tuint08 i=0;
			for(;i<commblock.dlc;i++)
			{
				TWIM_Write (commblock.data[i]);
			}
			TWIM_Stop();

			// not needed it is postwrite
			//twi_mhandler_write(&commblock);

			commblock.process -= PROCESSI2CWRITE;
			//comm ok
		}
	}
	if ( res != EI2CNONE)
	{
		//can_message dstrl = {0x23,0x00,I2CTEMPCANPORT,I2CTEMPCANPORT,0x06, {error,errdata->data[0],errdata->data[1],errdata->data[2],0x11,0x11}};
		//can_transmit(&dstrl);
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
