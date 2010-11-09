#include <lib3ds.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#include <lib3ds.h>

#ifndef TDS_DRAW_H
#define TDS_DRAW_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../building.h"

#define MDB_ADD 0x00
#define MDB_GET 0x01

#define EVP_ADD 0x00
#define EVP_GET 0x01

typedef struct
{
	float coords[3];
	char name[16];
} tds_evp_t;


typedef uint32_t tds_modelnum;
void* tds_modeldb (uint8_t in_command, void *in_data);
tds_modelnum tds_load_from_file (char* in_filename);
int tds_drawmodel (tds_modelnum in_modelnum);
#endif
