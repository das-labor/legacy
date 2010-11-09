#include <SDL.h>
#include <stdint.h>

typedef struct 
{
	uint8_t key;
	void (*fnc_p)(void);
} key_table_t;

static SDL_key key_table[256];
static uint8_t num_keys = 0;

uint8_t key_register (SDL_key in_key, void *in_func)
{
	
}

void keyhandler (SDL_keysim *in_key)
{
	uint8_t i = 0;

	for (i=0;i<num_keys;i++)
	{
		if (in_key->sym == key_table[i])
	}
}
