
#include <inttypes.h>

typedef struct{
	uint16_t zeroval;
	uint16_t minval;
	uint16_t maxval;
	uint16_t preheat;
	int8_t cal;
}pl_param_t;

void lampe_set(uint8_t lampe, uint8_t bright);
void lampe_dim(uint8_t lampe, int8_t d);
void lampe_init();

