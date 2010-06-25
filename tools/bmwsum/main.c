/* main.c */
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

#define  _LARGEFILE64_SOURCE
#define  _FILE_OFFSET_BITS 64
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#include "hash.h"

unsigned digest_size;
char mode_char = '*';
unsigned recursive = 1;


void process_dir(char* dirname);

char hextab[16] = { '0', '1', '2', '3',
                    '4', '5', '6', '7',
                    '8', '9', 'a', 'b',
                    'c', 'd', 'e', 'f' };

void print_digest(void* digest){
	unsigned i;
	for(i=0; i<digest_size; ++i){
		putchar(hextab[(((uint8_t*)digest)[i])>>4]);
		putchar(hextab[(((uint8_t*)digest)[i])&0xf]);
	}
}

void process_file(char* fname){
	int fd;
	struct stat stats;
	fd = open(fname, O_LARGEFILE);
	if(fd==-1){
		fprintf(stderr, "Error while opening file '%s'\n", fname);
		return;
	}
	fstat(fd, &stats);
	if(S_ISREG(stats.st_mode)){
		uint8_t digest[digest_size];
		hash_file(fd, stats.st_size, digest);
		close(fd);
		print_digest(digest);
		putchar(' ');
		putchar(mode_char);
		puts(fname);
	}else{
		close(fd);
		if(recursive && S_ISDIR(stats.st_mode)){
			process_dir(fname);
		}
	}
}

void process_stdin(void){
	uint8_t digest[digest_size];
	hash_stdin(digest);
	print_digest(digest);
	putchar(' ');
	putchar(mode_char);
	putchar('-');
}

void process_dir(char* dirname){
	DIR* dir;
	uint32_t dirname_size, fname_size;
	dirname_size = strlen(dirname);
	struct dirent *direntry;
	dir = opendir(dirname);
	if(dir==NULL){
		fprintf(stderr, "Error while opening directory '%s'\n", dirname);
		return;
	}
	while((direntry = readdir(dir))){
		fname_size = strlen(direntry->d_name);
		if(!strcmp(direntry->d_name, ".")){
			continue;
		}
		if(!strcmp(direntry->d_name, "..")){
			continue;
		}
		char name_buffer[dirname_size+fname_size+20];
		strcpy(name_buffer, dirname);
		if(name_buffer[dirname_size-1]!='/'){
			strcat(name_buffer, "/");
		}
		strcat(name_buffer, direntry->d_name);
		process_file(name_buffer);
	}
	closedir(dir);
}

int main(int argc, char** argv){
	uint32_t i;
	digest_size = digest_bytes();

	if(argc==1){
		process_stdin();
	}

	for(i=1; i<argc; ++i){
		process_file(argv[i]);
	}
	return 0;
}
