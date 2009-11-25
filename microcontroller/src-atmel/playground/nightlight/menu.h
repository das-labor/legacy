AVRX_EXTERNTASK(menu);

#define _TIMSK_TIMER1 TIMSK

typedef struct {
	void (*show)(void *data, uint8_t selected);
	void (*enter)(void *data);
	void *data;
	uint8_t type;
} menu_item_t;


typedef struct {
	char *name;
	uint8_t size;
	menu_item_t **items;
} menu_t;

typedef struct {
	char *name;
	uint8_t adr;
} mood_ctrl_t;

extern menu_t root;

