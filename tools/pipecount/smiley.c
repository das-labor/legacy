#include <stdio.h>
#include <unistd.h>

#define SAVE_CUR_POS "\x1B 7"
#define REST_CUR_POS "\x1B 8"
#define ERASE		 "\x1B[2K"

int main () {
    int i;
    static char smiley[]="-\\|/-\\|/";
	
	//fprintf (stderr, "%s", SAVE_CUR_POS);

    for(i = 0; i < 100; i++) {
        usleep(100000);
		fprintf (stderr, "%s", ERASE);
        fprintf(stderr,"\rwait... %c",smiley[ i%10 ]);
       // rewind(stderr);
    }

    return 0;
}

