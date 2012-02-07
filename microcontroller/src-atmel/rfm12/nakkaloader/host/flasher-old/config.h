#ifndef NF_CONFIG_H
#include <stdint.h>
#include <stdlib.h>

/* struct to store local config data */
typedef struct {
	/* device/protocol-related */
	uint16_t pagesize;      /* amount of bytes per page */
	uint32_t dstaddr;       /* id of the target */
	uint32_t myaddr;        /* our address */
	uint_fast8_t  addrlen;       /* length of address field the target uses (in bits) */
	uint_fast8_t  rxbufsize;     /* size of the target's buffer */
	uint_fast8_t  protooffset;   /* offset before the nakkaloader protocol header starts */
	uint_fast8_t  chunksize;     /* size of a single chunk we may send to the target */
	uint_fast32_t numpages;      /* number of pages */

	/* program-related stuff */
	uint8_t verbosity;

	char* fname;
	size_t fname_len;

} nf_flashconfig_t;

typedef struct
{
	uint_fast8_t addrsize;
	uint_fast32_t addr;
	size_t fname_len;
	char *fname;
	uint8_t verbosity;
} nf_config_t;

// uint8_t nf_parse_args (int in_argc, char **in_argv, nf_flashconfig_t *out_conf);

#define NF_CONFIG_H
#endif
