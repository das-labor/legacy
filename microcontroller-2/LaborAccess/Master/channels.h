
#include <stdint.h>
#include <avrx.h>

#define NUM_CHANNELS 1
#define CHANNEL_SERVER1 0

typedef struct{
	uint8_t * buffer;
	uint8_t buffer_size;
	uint8_t size;
	Mutex mutex;
}channel_t;

extern channel_t Channels[NUM_CHANNELS];

