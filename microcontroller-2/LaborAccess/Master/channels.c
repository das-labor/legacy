#include "channels.h"
uint8_t client_buf[20];

channel_t Channels[NUM_CHANNELS] ={
	{client_buf, sizeof(client_buf), 0, SEM_PEND},
};

