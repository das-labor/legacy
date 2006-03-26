#ifndef CANTUN_H
#define CANTUN_H

/////////////////////////////////////////////////////////////////////////////
// Types

typedef enum {
		CanTunReset,
		CanTunVersion,
		CanTunPacket,
		CanTunFilter,
		CanTunStats
} CanTunType;


typedef struct {
	MessageControlBlock mcb;
	CanTunType          type;
} CanTunMsg;

typedef struct {
	MessageControlBlock mcb;
	CanTunType          type;

	char name[8];
	char version[8];
} CanTunMsgVersion;

typedef struct {
	MessageControlBlock mcb;
	CanTunType          type;

	uint32_t id;
    uint8_t  data[8];
	uint8_t  flags;
} CanTunMsgPacket;

typedef struct {
	MessageControlBlock mcb;
	CanTunType          type;

	uint32_t filter[4];
	uint32_t mask;
} CanTunMsgFilter;

/////////////////////////////////////////////////////////////////////////////
// Interface
void CanTunInit();
void CanTunSend(CanTunMsg *);
CanTunMsg *CanTunDequeue();



#endif
