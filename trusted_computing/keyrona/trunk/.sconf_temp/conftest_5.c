

#include "time.h"

int
main() {
  clock_gettime( CLOCK_MONOTONIC, (struct timespec*)0 );
return 0;
}
