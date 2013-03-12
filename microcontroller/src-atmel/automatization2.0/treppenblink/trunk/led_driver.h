

enum {
BLUE,
GREEN,
RED,
NUM_COLORS
} led_colors_t;


// Dots need BGR Format
typedef struct {
	uint16_t blue;
	uint16_t green;
	uint16_t red;
} dot_t;

extern void pushDot(dot_t *dot);

