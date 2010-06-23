/* main_bmw224.c */
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

#include "bmw_small.h"

#define HASH_BLOCKSIZE BMW_SMALL_BLOCKSIZE_B
#define READ_BLOCKS (READ_BLOCKSIZE/HASH_BLOCKSIZE)

void hash_file(int fd, uint64_t fsize, void* digest){
	uint64_t main_blocks;
	uint32_t last_blocks, last_bytes;
	uint32_t i;
	uint8_t buffer[READ_BLOCKSIZE];
	uint8_t *bufptr;
	bmw_small_ctx_t hctx;

	main_blocks = fsize/READ_BLOCKSIZE;
	last_blocks = (fsize%READ_BLOCKSIZE)/HASH_BLOCKSIZE;
	last_bytes  = fsize%HASH_BLOCKSIZE;
	bmw224_init(&hctx);
	while(main_blocks--){
		read(fd, buffer, READ_BLOCKSIZE);
		i=READ_BLOCKS;
		bufptr = buffer;
		do{
			bmw_small_nextBlock(&hctx, bufptr);
			bufptr += HASH_BLOCKSIZE;
		}while(--i);
	}
	if(last_blocks){
		read(fd, buffer, last_blocks*HASH_BLOCKSIZE);
		i=last_blocks;
		bufptr = buffer;
		do{
			bmw_small_nextBlock(&hctx, bufptr);
			bufptr += HASH_BLOCKSIZE;
		}while(--i);
	}
	read(fd, buffer, last_bytes);
	bmw_small_lastBlock(&hctx, buffer, last_bytes*8);

	bmw224_ctx2hash(digest, &hctx);
}

void hash_stdin(void *digest){
	uint8_t buffer[READ_BLOCKSIZE];
	size_t read_size, offset=0;
	int32_t i;
	bmw_small_ctx_t hctx;
	FILE *f;

	bmw224_init(&hctx);
	f = stdin;
	while(!feof(f)){
		read_size = fread(buffer+offset, 1, READ_BLOCKSIZE-offset, f);
		for(i=0; i<read_size-HASH_BLOCKSIZE; i+=HASH_BLOCKSIZE){
			bmw_small_nextBlock(&hctx, buffer+i);
		}
		if(read_size-i){
			memmove(buffer, buffer+i, read_size-i);
			offset=read_size-i;
		}else{
			offset=0;
		}
	}
	fclose(f);
	bmw_small_lastBlock(&hctx, buffer, read_size*8);
	bmw224_ctx2hash(digest, &hctx);
}

uint32_t digest_bytes(void){
	return 224/8;
}
