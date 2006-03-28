AVRX_EXTERNTASK(menu);

#define _TIMSK_TIMER1 TIMSK

typedef struct {
	void (*show)(void *data, uint8_t selected);
	void (*enter)(void *data);
	void *data;
} menu_item_t;


typedef struct {
	char *name;
	uint16_t size;
	menu_item_t **items;
} menu_t;

extern menu_t root;

extern void menu_handler(void *data);
extern void show_menu(void *data, uint8_t selected);
extern void menu_update(uint8_t act);
extern void show_text(void* data, uint8_t selected);
