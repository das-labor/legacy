

enum {
	BLUE,
	GREEN,
	RED,
	NUM_COLORS
} led_colors_t;


// Dots need BGR Format
#ifdef LED_12BIT
typedef struct {
	uint16_t blue;
	uint16_t green;
	uint16_t red;
} dot_t;
#else
typedef struct {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
} dot_t;
#endif

extern void pushDot(dot_t *dot);
extern void updateDots(void);

