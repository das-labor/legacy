#ifndef NF_CONFIG_H

#include <stdint.h>

typedef struct
{
	uint_fast8_t addrsize;
	uint_fast32_t addr;
	size_t fname_len;
	char *fname;
	uint8_t verbosity;
} nf_config_t;

#define NF_CONFIG_H
#endif
