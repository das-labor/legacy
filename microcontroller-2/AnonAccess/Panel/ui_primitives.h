#ifndef UI_PRIMITIVES_H_
#define UI_PRIMITIVES_H_


typedef struct menu_st{
	PGM_P name;                 /* 2 byte */
	enum {none, 
		  autosubmenu,
		  submenu, 
		  execute, 
		  back, 
		  terminator} options;  /* 1 byte*/
	union { 
		void (*fkt)(void);
		void* submenu; } x;      /* 2 byte */
} menu_t;


void ui_primitives_init(void);
void print_progressbar(double percent, uint8_t xpos, uint8_t ypos, uint8_t width);
uint8_t radioselect(char* opts);
uint8_t radioselect_P(PGM_P opts);
void checkselect(char* opts, uint8_t* config);
void checkselect_P(PGM_P opts, uint8_t* config);
void menuexec(menu_t* menu);
void draw_frame(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char framechar);
void ui_hexdump(const void* data, uint16_t length);

#endif /*UI_PRIMITIVES_H_*/
