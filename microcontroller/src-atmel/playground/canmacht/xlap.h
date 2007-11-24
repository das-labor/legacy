#include "lap.h"

AVRX_EXTERNTASK(laptask);

extern uint8_t myaddr;

void xlap_init();


struct sensor_struct {
		uint8_t typ;
		uint8_t wert;
		struct sensor_struct *next;
};

typedef struct sensor_struct sensor_t;
extern sensor_t *sensor;
