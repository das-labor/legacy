#include "channel.h"
#include "channels.h"


/****** Actual Channels **********/

uint8_t client_buf[42];

channel_t Channels[NUM_CHANNELS] ={
	{client_buf, sizeof(client_buf), 0, {0, SEM_DONE}, 0},
};


