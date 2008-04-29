#ifndef UI_PRIMITIVES_H_
#define UI_PRIMITIVES_H_

/* we use a integer instead of void* 'cause the compiler keepr rumoring about 
 * data_ptr <-> func_ptr conversation 
 */
typedef uint16_t superp;

typedef struct menu_st{
	PGM_P name;                 /* 2 byte */
	enum {none, 
		  autosubmenu,
		  submenu, 
		  execute, 
		  back, 
		  terminator} options;  /* 1 byte*/
	superp x;
//	void* x;
//	union { 
//		void (*fkt)(void);
//		void* submenu; } x;      /* 2 byte */
} menu_t;


#define LCD_LABORLOGO     7
#define LCD_RADIOSELECT   5
#define LCD_CHECKSELECT   6
#define LCD_RADIONOSELECT 'o'
#define LCD_CHECKNOSELECT 0xDB

#define LCD_FULLCHAR  255
#define LCD_EMPTYCHAR  ' '
#define LCD_RARROW 0x7E
#define LCD_LARROW 0x7F

#define NO_KEY     ' '
#define ENTER_KEY  'F'
#define SELECT_KEY 'E'
#define UP_KEY     '0'
#define DOWN_KEY   'C'


void ui_primitives_init(void);
void print_progressbar(double percent, uint8_t xpos, uint8_t ypos, uint8_t width);
uint8_t radioselect(const char* opts);
uint8_t radioselect_P(PGM_P opts);
void checkselect(const char* opts, uint8_t* config);
void checkselect_P(PGM_P opts, uint8_t* config);
void menuexec(menu_t* menu);
void draw_frame(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char framechar);
void ui_hexdump(const void* data, uint16_t length);
void ui_hexdump_P(PGM_VOID_P data, uint16_t length);
void ui_textwindow(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char* text);
void ui_textwindow_P(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, PGM_P text);


#endif /*UI_PRIMITIVES_H_*/
