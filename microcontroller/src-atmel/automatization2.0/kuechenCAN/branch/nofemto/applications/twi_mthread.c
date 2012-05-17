#include "femtoos_code.h"
#ifdef TWI_MTHREAD
#include "twi_master.h"


// user must implement those handler
extern void twi_mhandler_read(i2c_message *data);
extern void twi_mhandler_write(i2c_message *data);
extern void twi_mhandler_error(Tuint08 error,i2c_message *data);

// user has to define commblock
extern i2c_message commblock;

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
	  twi_mhandler_write(&commblock);
	  commblock.process -= PROCESSI2CWRITE;
	  //comm ok
	}
    }
  if ( res != EI2CNONE)
    {
      twi_mhandler_error(res,&commblock);
    }
}

#if (preTaskDefined(twim))
void appLoop_twim(void)
{
  while(true)
    {
      twi_mhandler(); 
      taskDelayFromNow(20);
    }
}
#endif // preTaskDefined
#endif
