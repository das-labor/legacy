#define CONF_TERMWIDTH 0x0001
#define CONF_DUMPMODE  0x0002
	#define CONF_DUMPMODE_HEX   0x00
	#define CONF_DUMPMODE_ASCII 0x01

extern void *conf_handler (uint_fast8_t in_idx, void* in_val);
