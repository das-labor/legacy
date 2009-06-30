/* bin2avrS.c */
/*
    This file is part of the AVR-Huffman.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define BYTES_PER_LINE 16

void string2resname(char* s){
	while(*s){
		if(!isalnum(*s))
			*s = '_';
		++s;
	}
}

int main(int argc, char** argv){
	unsigned i,j;
	uint8_t buffer[BYTES_PER_LINE];
	size_t  bytes_read;
	FILE *fin, *fout;
	for(i=1; i<argc; ++i){
		char foutname[strlen(argv[i])+5];
		char resname[strlen(argv[i])+5];
		strcpy(foutname, argv[i]);
		strcat(foutname, ".S");
		strcpy(resname, argv[i]);
		string2resname(resname);
		fin = fopen(argv[i], "r");
		if(fin==NULL){
			fprintf(stderr, "could not open file '%s' for reading!\n", argv[i]);
			continue;
		}
		fout = fopen(foutname, "w");
		if(fout==NULL){
			fprintf(stderr, "could not open file '%s' for writing!\n", foutname);
			fclose(fin);
			continue;
		}
		fprintf(fout, "; converted by bin2avrS %s -> %s\n"
		              ".global %s \n%s:\n",
				argv[i], foutname, resname, resname);
		
		do{
			bytes_read = fread(buffer, 1, BYTES_PER_LINE, fin);
			if(bytes_read==0)
				break;
			fputs("\n .byte", fout);
			for(j=0; j<bytes_read; ++j){
				fprintf(fout, " 0x%2.2X", buffer[j]);
				if(j+1!=bytes_read)
					fputc(',', fout);
			}
		}while(bytes_read==BYTES_PER_LINE);
		fputc('\n', fout);
		fclose(fin);
		fclose(fout);
	}
	return 0;
}
