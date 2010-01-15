#define LCD_LED_ON true
#define LCD_LED_OFF false

void lcdBacklight(bool led);

enum Room {
	test, blue, yellow, red //room identifier for blinds
};

typedef struct {
	uint8_t ID; //ident of room
	int16_t position; //if <0 all blinds can move individually. if 0-255 all blind positions of the room are overwritten to this
} room;

typedef struct {
	uint8_t ID; //ident of each blind
	uint8_t room; //room in which the blind is installed
	uint8_t xpos; //X position on LCD
	uint8_t ypos; //Y position on LCD
	uint8_t barWidth; //width of bar on LCD in px
	uint8_t barHeight; //height of bar on LCD in px
	uint8_t barPos; //current position of bar on LCD (0 to barHeight)
	uint8_t realWidth; //width in sections (will be used later...)
	uint8_t realHeight; //max height (count variable)
	uint8_t realPos; //position of blind (0 to realHeight)
	int8_t dir; //direction can be positive (up) and negative (down) or neutral (zero)
	bool locked; //blind can be locked from extern
	bool visible; //draw on LCD or not
	bool scale; //draw scale in bar
} blindBarType;
