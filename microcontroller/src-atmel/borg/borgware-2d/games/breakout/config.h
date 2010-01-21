#ifndef CONFIG_H
#define CONFIG_H
/* amount of speed to slow down on bounce */
#define BOUNCE_SLOWDOWN 8

/* minimum speed of the ball */
#define BALL_MINSPEED 64
#define BALL_MAXSPEED 224

/* initial amount of lifes */
#define START_LIFES 3

/* rebound size */
#define REBOUND_SIZE 4

/* rebound reflection: values to add to the vector at rebound field n
 * note: directions are inverted
 */
static int8_t rebound_reflection[6][2] =
{
	{-54,-20},  /* offside */
	{-32,-12},
	{-16, -8}, /* side ... middle */
	{16,  -8},
	{32, -12},
	{54, -20}
};

/* "color" of the rebound */
#define REBOUND_COLOR 2

#endif /* CONFIG_H */
