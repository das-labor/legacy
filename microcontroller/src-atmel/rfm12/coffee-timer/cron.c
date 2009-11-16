#include <stdint.h>
#include "cron.h"
#include "lcd.h"

static cronjob_t myjobs[NUM_CRONJOBS];

uint8_t cron_add (uint16_t in_time, void ((*in_function)()), uint16_t in_param)
{
	uint8_t i = 0;

	for (;i<NUM_CRONJOBS;i++)
	{
		if (myjobs[i].time != 0) continue;

		myjobs[i].time = in_time;
		myjobs[i].func = in_function;
		myjobs[i].func_param = in_param;

		return 1;
	}

	return 0;
}

void cron_init()
{
	uint8_t i=0;

	for (i=0;i<NUM_CRONJOBS;i++)
		myjobs[i].time = 0;
}

uint8_t task_queue (void ((*in_func)(uint16_t)), uint16_t in_param)
{
	static void ((*myfunc)(uint16_t)) = 0; // ((void *)()) 0;
	static uint16_t myparam;
	
	if (in_func != 0) /* add a function */
	{
		if (myfunc != 0) return 0;

		myfunc = in_func;
		myparam = in_param;

		return 1;
	}
	
	/* execute a function */
	if (myfunc == 0) return 0;

	myfunc(myparam);
	myfunc = 0;

	return 1;
}

void cron_tick()
{
	uint8_t i;

	for (i=0;i<NUM_CRONJOBS;i++)
	{
		if (!myjobs[i].time) continue;

		myjobs[i].time--;

		if (!myjobs[i].time)
		{
			if (!task_queue(myjobs[i].func, myjobs[i].func_param))
				myjobs[i].time = 1;
		}
	}
}
