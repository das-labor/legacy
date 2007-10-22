#include "airlab.h"

#define AL_STATE_NEW 0x00
#define AL_STATE_AWAING_PONG 0x01
#define AL_STATE_PONG_RECEIVED 0x02
#define AL_STATE_RETR 0xfe
#define AL_STATE_DEL 0xff

typedef uint8_t airlab_state;

typedef struct
{
	airlab_addr addr;
	airlab_state state;
} airlab_state;
