#define CMD_RESET		0x7A
#define CMD_PING		0x01
#define CMD_PONG		0x81
#define CMD_ALIVE		0x42
#define CMD_STATE		0x19
#define CMD_STATE_SET   	0x21
#define CMD_MOTION		0x22
#define CMD_MOTION_TRESH_SET	0x25
#define CMD_MAIL_SENSOR		0x28
#define CMD_PWRFAIL		0x60
#define CMD_LOW_POWER		0x61 /* NOTE: the low power indicator is non-fatal, while the PWRFAIL cmd may trigger an alarm */
#define CMD_ERROR		0xEE
#define PROTO_TYPE0		0x09
#define PROTO_TYPE1		0x12

#define RFM12_PROTO_TYPE 0xA0

/* indices for the packet array */
#define F_T0 0
#define F_T1 (F_T0+1)
#define F_SRC (F_T1+1)
#define F_DST (F_SRC+1)
#define F_CMD (F_DST+1)
#define F_DATA (F_CMD+1)

/* minimum length of a packet */
#define PACKET_MINLEN	F_DATA+1

/* broadcast addr */
#define ADDR_BCAST 0xff

typedef struct alert_packet_s
{
	uint8_t len; /* this is the len of the payload (rfm12_rx_len() - PACKET_MINLEN) */
	uint8_t cmd;
	uint8_t src;
	uint8_t dst;
	uint8_t *payload;
} alert_packet_t;


/*
  packet: TYPE0 TYPE1 SRC DST COMMAND [other stuff]
 */
