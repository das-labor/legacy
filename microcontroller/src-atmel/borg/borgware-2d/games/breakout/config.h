#ifndef CONFIG_H
#define CONFIG_H
/* amount of speed to slow down on bounce */
#define BOUNCE_SLOWDOWN 4

/* minimum speed of the ball */
#define BALL_MINSPEED 64
#define BALL_MAXSPEED 224

/* initial amount of lifes */
#define START_LIFES 3

/* rebound size */
#define REBOUND_SIZE 4


/* "color" of the rebound */
#define REBOUND_COLOR 2

/* rebound reflection: values to add to the vector at rebound field n
 * the size of this array must be REBOUND_SIZE +2
 */
static const int8_t rebound_reflection[6][2] =
{
	{-72, -20},  /* offside left */
	{-40, -12},  /* left */
	{-16,  -8},  /* center left */
	{ 16,  -8},  /* center right */
	{ 40, -12},  /* right */
	{ 72, -20}   /* offside right */
};

#define DEMO_CYCLES 1200;

#endif /* CONFIG_H */

