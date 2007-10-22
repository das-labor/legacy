typedef uint8_t airlab_addr;
typedef uint8_t airlab_proto;

typedef struct
{
	uint8_t len;
	airlab_addr src;
	airlab_arr dst;
	uint8_t type;

} airlab_packet;

#ifdef AL_PROTO_PING
	#define AL_TYPE_PING
	#define AL_TYPE_PONG
#endif

// #define AL_TYPE_ ...

#define AL_SRC(a) (*a[3])
#define AL_DST(a) (*a[2])
#define AL_LEN(a) (*a[0])
#define AL_TYPE(a) (*a[1])
