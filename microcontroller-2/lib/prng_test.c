/* test for prng.h */


#include <stdio.h>
#include "prng.h"

int main(int argc, char** argv){
	unsigned long i,j, cycles=5000, statistics[256]={0};

	if(argc==2){
		cycles=atol(argv[1]);
	}

	for(i=0;i<cycles;++i){
		++statistics[random8()];
	}

	/* print the statistics */
	printf("after %lu cycles: \n",cycles);
	for(i=0;i<256/8;++i){
		for(j=0; j<8;++j){
			printf("%3lu: %5lu\t",i*8+j,statistics[i*8+j]);
		}
		puts("");
	}

}


