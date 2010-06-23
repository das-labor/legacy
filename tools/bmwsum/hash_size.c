/* main_bmw256.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "hash.h"
#ifndef _BSD_SOURCE
  #define _BSD_SOURCE
#endif
#include <endian.h>


#define HASH_BLOCKSIZE BMW_SMALL_BLOCKSIZE_B
#define READ_BLOCKS (READ_BLOCKSIZE/HASH_BLOCKSIZE)

void hash_file(int fd, uint64_t fsize, void* digest){
	*((uint64_t*)digest) = htobe64(fsize);
}

void hash_stdin(void *digest){
	uint64_t read_size=0;
	uint8_t buffer[READ_BLOCKSIZE];
	FILE *f;

	f = stdin;
	while(!feof(f)){
		read_size += fread(buffer, 1, READ_BLOCKSIZE, f);
	}
	fclose(f);
	*((uint64_t*)digest) = htobe64(read_size);
}

uint32_t digest_bytes(void){
	return 64/8;
}
