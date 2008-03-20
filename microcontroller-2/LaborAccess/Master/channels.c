#include "channel.h"
#include "channels.h"


/****** Actual Channels **********/

uint8_t server1_buf[42];

channel_t Channels[NUM_CHANNELS] ={
	{server1_buf, sizeof(server1_buf), 0, {0, SEM_DONE}, 0},
};

