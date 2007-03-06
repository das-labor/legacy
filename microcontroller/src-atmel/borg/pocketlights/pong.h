#define PADDLE_LENGTH 3
#include "pixel.h"

void play_pong();

typedef struct {
	pixel pos;
	Ddirection dir;
	int speed;
} ball_t;

