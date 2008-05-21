/* main-crygop.c */
/*
    This sourcefile is part of the crygop programm.
    Copyright (C) 2008  Daniel Otte

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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "crygop.h"

#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
 uint8_t interim=0;
#endif


#define VER_MAJOR 0
#define VER_MINOR 1


#define MAX_N 0x3FFFFFFFL


#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
 #define USAGE_STRING \
 "crygop usage:\n" \
 " crygop [-v | --verbose] [[-k | --key] <keysting>] [[-e | --encrypt] | [-d | --decrypt] | [ -t | --test]]\n" \
 "        [[-r | --rounds] <rounds>] [-i | --interim] [--version] [--license] [<m> [<x1> [<x2> ...]]]\n" \
 " \n" \
 " crygop is free software and licensed under GPL version 3, see crygop -l for more information\n"
#else
 #define USAGE_STRING \
 "crygop usage:\n" \
 " crygop [-v | --verbose] [[-k | --key] <keysting>] [[-e | --encrypt] | [-d | --decrypt] | [ -t | --test]]\n" \
 "        [[-r | --rounds] <rounds>] [--version] [--license] [<m> [<x1> [<x2> ...]]]\n" \
 " \n" \
 " crygop is free software and licensed under GPL version 3, see crygop -l for more information\n"
#endif

#define LICENSE_STRING \
    "crygop - a program to cryptographically secure permute positive integers \n" \
    "Copyright (C) 2008  Daniel Otte\n" \
    "\n" \
    "This program is free software: you can redistribute it and/or modify\n" \
    "it under the terms of the GNU General Public License as published by\n" \
    "the Free Software Foundation, either version 3 of the License, or\n" \
    "(at your option) any later version.\n" \
    "\n" \
    "This program is distributed in the hope that it will be useful,\n" \
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" \
    "GNU General Public License for more details."

void print_usage(void){
	puts(USAGE_STRING);
}

void print_license(void){
	puts(LICENSE_STRING);
}


void print_version(void){
	printf("crygop v%d.%d compiled at %s %s", VER_MAJOR, VER_MINOR, __DATE__, __TIME__);
	#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
	printf(" with support for interim result printing");
	#endif
	puts("");
}

void print_enc(uint32_t x, crygop_ctx_t *ctx){
	printf(" %4u", x);
	x =crygop_enc(x, ctx);
	#ifdef CRYGOP_SUPPORT_INTERIMRESULTS	
	if(interim==0)
		printf(" -> %4u\n", x);
	putchar('\n');
	#else
	printf(" -> %4u\n", x);
	#endif
}


void print_dec(uint32_t x, crygop_ctx_t *ctx){
	printf(" %4u", x);
	x =crygop_dec(x, ctx);
	#ifdef CRYGOP_SUPPORT_INTERIMRESULTS	
	if(interim==0)
		printf(" <- %4u\n", x);
	putchar('\n');
	#else
	printf(" <- %4u\n", x);
	#endif
}


uint8_t print_test(uint32_t x, crygop_ctx_t *ctx){
	/* encrypt step */	
	uint32_t x2=x;	
	printf(" %4u", x);
	x = crygop_enc(x, ctx);
	#ifdef CRYGOP_SUPPORT_INTERIMRESULTS	
	if(interim==0){
		printf(" -> %4u", x);
	}
	#else
	printf(" -> %4u", x);
	#endif
	/* decrypt step */
	x = crygop_dec(x, ctx);
	#ifdef CRYGOP_SUPPORT_INTERIMRESULTS	
	if(interim==0){
		printf(" <- %4u\n", x);
	}
	putchar('\n');	
	#else
	printf(" <- %4u\n", x);
	#endif
	return (x==x2)?1:0;
}

int main(int argc, char** argv){
	unsigned i=0;
	uint32_t n=101;
	uint32_t rounds=0;
	crygop_ctx_t ctx;
	char *gpl = "GPLv3, (c)Daniel Otte";
	char *key = NULL; 	
	uint32_t x;
	uint32_t *a = NULL;
	int32_t  aidx=-1;
	enum{test, encrypt, decrypt} mode=test;
	uint8_t options_parsed=0;	
	uint8_t verbouse=0;
	
	/* BEGIN of parameter parsing */

	while(i<argc-1){
		if(!options_parsed && argv[1+i][0]=='-'){
			if(strcmp(argv[1+i],"--version")==0){
				print_version();
				return 0;			
			}
			if(strcmp(argv[1+i],"--license")==0){
				print_license();
				return 0;			
			}
			if(strcmp(argv[1+i],"--help")==0 || strcmp(argv[1+i],"-h")==0 || strcmp(argv[1+i],"-?")==0){
				print_usage();
				return 0;			
			}
			if(strcmp(argv[1+i],"--verbouse")==0 || strcmp(argv[1+i],"-v")==0){
				verbouse=1;		
				++i;
				continue;			
			}
			if(strcmp(argv[1+i],"--key")==0 || strcmp(argv[1+i],"-k")==0){
				if(argc<3+i){
					print_usage();
					return -1;
				}
				key=argv[2+i];
				i+=2;	
				continue;		
			}
			if(strcmp(argv[1+i],"--encrypt")==0 || strcmp(argv[1+i],"-e")==0){
				mode=encrypt;
				++i;
				continue;			
			}
			if(strcmp(argv[1+i],"--decrypt")==0 || strcmp(argv[1+i],"-d")==0){
				mode=decrypt;
				++i;
				continue;			
			}
			if(strcmp(argv[1+i],"--test")==0 || strcmp(argv[1+i],"-t")==0){
				mode=test;
				++i;
				continue;			
			}
			if(strcmp(argv[1+i],"--rounds")==0 || strcmp(argv[1+i],"-r")==0){
				if(argc<3+i){
					print_usage();				
					return -1;
				}
				rounds=strtol(argv[2+i], NULL, 0);
				i+=2;
				continue;			
			}

			#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
			if(strcmp(argv[1+i],"--interim")==0 || strcmp(argv[1+i],"-i")==0){
				interim=1;;
				++i;
				continue;			
			}
			#endif
			options_parsed = 1;
		} else {
			options_parsed = 1;
			if(aidx==-1){
				a=malloc((argc-i-1)*sizeof(uint32_t));
				if(a==NULL){
					puts("malloc error !!!");
					return -3;
				}	
				aidx=0;
				n=strtol(argv[1+i], NULL, 0);
				if(n<=2 || n>MAX_N){				
					print_usage();
					return -1;
				}
			}else{
				a[aidx++]=strtol(argv[1+i], NULL, 0);
				if(a[aidx-1]>=n){
					print_usage();
					return -1;
				}
			}
			++i;
		}
	}

	/* END of parameter parsing */

	if(n==0){
		print_usage();
		return -1;
	}

	if(key==NULL)
		key = gpl;

	if(rounds==0)
		rounds=8;
	
	crygop_generate_ctx(key, strlen(key)*8, n, rounds, &ctx);
	if(verbouse)
		print_ctx(&ctx);

	if(aidx<1){
		for(i=0; i<n; ++i){
			x = i;
			switch(mode){
				case decrypt:
					print_dec(x, &ctx);
					break;
				case encrypt:
					print_enc(x, &ctx);
					break;
				case test:
				default: 				
					if(print_test(x, &ctx)==0){
						puts("ERROR dec(enc(x)) != x !!!");
						return -2;					
					}				
					break;
			}
		}
	} else {
		for(i=0; i<aidx; ++i){
			x = a[i];
			switch(mode){
				case decrypt:
					print_dec(x, &ctx);
					break;
				case encrypt:
					print_enc(x, &ctx);
					break;
				case test:
				default: 				
					if(print_test(x, &ctx)==0){
						puts("ERROR dec(enc(x)) != x !!!");
						return -2;					
					}				
					break;
			}
		}
	}
	crygop_free_ctx(&ctx);
	return 0;
}


