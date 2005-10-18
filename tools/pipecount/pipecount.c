// pipecount.c
// entirely written in ANSI-C
/*
 This Program does the following:
 it reads Data from STDIN and passes it to STDOUT,
 while data is passing the bytes are counted, and the amount is written to a specified file.
 Command-line:
 pipecount [-b|-K|-k|-M|-m|-G|-g|-T|-t|-E|-e|-H|-h] [-a|-o|-s] [-[-]] [<file>]
 b:	byte
 K:	kilo-byte (1024 byte)
 k:	1000=10**3 byte
 M:	mega-byte (1024 kilo-byte)
 m:	10**6 byte
 G:	giga-byte (1024 mega-byte)
 g:	10**9 byte
 T:	terra-byte (1024 giga-byte)
 t:	10**12 byte
 E:	eta-byte (1024 terra-byte)
 e:	10**15 byte

 H:	automatic scaling on 1024-base (default)
 h:	automatic scaling on 1000-base

 a:	append to output file (default if output goes to stderr)
 o:	always overwrite output file (default if output does not go to stderr)
 s: write output in a termianl-friendly way (is off by default)
 <file>:	File where the output is written to (if not specified -> stderr)

*/
/*
Author: Daniel Otte
email: daniel.otte@rub.de
License: GPL
Date: 21.03.2005
*/

/*
History (DD.MM.YYYY):
16.03.2005:	first release as byte-count to discuss@das-labor.org
		containig a few stupid mistakes wich luckily didn't cause bugs
21.03.2005:	increasing speed by avoiding if-constructs by using multiple functions
		and pointer to functions
17.10.2005:	committed to labor svn
18.10.2005:	replaced exit by return to avoid warning
*/

/*
Todo:
 * speed-up
   * support for kernel functions
   * replace printf by own function
 * multi-threading
 * advancing Output for stderr on terminals
 * showing rates
*/




#include <stdio.h>

/* constants */
#define BUFFER_SIZE 10240
#define OUTFORMAT "%'20.3f %c\n"
#define true 1
#define false 0
#define TERM_LN_ERASE "\x1B[2K"
#define TERM_CUR_UP1 "\x1B[A"
#define TERM_CUR_UP (s) "\x1B["#(S)"A"

/* type definitions*/
typedef int bool;
typedef unsigned char byte;



/* global vars */
    unsigned long long count=0;
    FILE *f;
    unsigned long long divisor=1024;
	char suffix='b';
    char *suf_list="bKMGTE";


/* func declarations */
void writer_auto_append();
void writer_auto_overwr();
void writer_auto_term();
void writer_manu_append();
void writer_manu_overwr();
void writer_manu_term();

/* func implementations */
void writer_auto_overwr(){
     static int t_div=1;
     static int suf_index=0;
     
     while (count/t_div >= divisor){
		t_div *= divisor;
		suf_index = (suf_index+1) % 6;
     }
     fprintf (f, OUTFORMAT,(double)(((double)count)/((double)t_div)), suf_list[suf_index]);
     fseek(f, 0, SEEK_SET);
}


void writer_auto_append(){
     static int t_div=1;
     static int suf_index=0;
     
     while (count/t_div >= divisor){
		t_div *= divisor;
		suf_index = (suf_index+1) % 6;
     }
     fprintf (f, OUTFORMAT,(double)(((double)count)/((double)t_div)), suf_list[suf_index]);
}


void writer_auto_term(){
     static int t_div=1;
     static int suf_index=0;
     
     while (count/t_div >= divisor){
		t_div *= divisor;
		suf_index = (suf_index+1) % 6;
     }
	 fprintf (f, "%s\r", TERM_LN_ERASE),
     fprintf (f, OUTFORMAT,(double)(((double)count)/((double)t_div)), suf_list[suf_index]);
	 fprintf (f, "%s", TERM_CUR_UP1);
}


void writer_manu_overwr(){
     fprintf (f, OUTFORMAT,(double)(((double)count)/((double)divisor)), suffix);
     fseek(f, 0, SEEK_SET);
}


void writer_manu_append(){
     fprintf (f, OUTFORMAT,(double)(((double)count)/((double)divisor)), suffix);
}


void writer_manu_term(){
	 fprintf (f, "%s\r", TERM_LN_ERASE);
     fprintf (f, OUTFORMAT,(double)(((double)count)/((double)divisor)), suffix);
	 fprintf (f, "%s", TERM_CUR_UP1);

}


/*
* The main-function
*/
int main(int argc, char **argv) {

    bool append=false;		//default is overwrite
	bool term=false;		//default is no terminal output
    bool automatic=true;	//default is to automatically scale

    byte buffer[BUFFER_SIZE];
    unsigned int bytes_in_buffer=0;
    void (*writer) ();
    {
	int i=0;
	while ((++i < argc) && (argv[i][0] == '-') && (argv[i][1] != '-') && (argv[i][1] != '\0')){
	   int j=0;
	   while (argv[i][++j]) {
		switch (argv[i][j]) {
		    case 'b': suffix=argv[i][j]; automatic = false; divisor = 1; break;
		    case 'k': suffix=argv[i][j]; automatic = false; divisor = 1000; break;
		    case 'K': suffix=argv[i][j]; automatic = false; divisor = 1024; break;
		    case 'm': suffix=argv[i][j]; automatic = false; divisor = 1000 * 1000; break;
		    case 'M': suffix=argv[i][j]; automatic = false; divisor = 1024 * 1024; break;
		    case 'g': suffix=argv[i][j]; automatic = false; divisor = 1000 * 1000 * 1000; break;
		    case 'G': suffix=argv[i][j]; automatic = false; divisor = 1024 * 1024 * 1024; break;
		    case 't': suffix=argv[i][j]; automatic = false; divisor = 1000LL * 1000LL * 1000LL * 1000LL; break;
		    case 'T': suffix=argv[i][j]; automatic = false; divisor = 1024LL * 1024LL * 1024LL * 1024LL; break;
		    case 'e': suffix=argv[i][j]; automatic = false; divisor = 1000LL * 1000LL * 1000LL * 1000LL * 1000LL; break;
		    case 'E': suffix=argv[i][j]; automatic = false; divisor = 1024LL * 1024LL * 1024LL * 1024LL * 1024LL; break;

		    case 'h': automatic = true; divisor = 1000; suf_list="bkmgte"; break;
		    case 'H': automatic = true; divisor = 1024; suf_list="bKMGTE"; break;

		    case 'a': append = true;  break;
		    case 'o': append = false; break;
			case 's': term	 = true;  break;

		    default: /* you did something wrong! */ break;  
		} // switch (argv[i][j])
	   } // (argv[i][++j]) 
	} // while ((++i < argv) && (argv[i][0] == '-') && (argv[i][1] != '-') && (argv[i][1] == '\0'))
	

	
	if (i<argc) { // there must be at least one argument left
	    if (! (f=fopen(argv[i],append?"a+":"w+")) )
		f = stderr;
	} else {
	    f = stderr;
	}
	
	if (f == stderr){
	    append = true;
		term = true;
	}
	
	// select the correct writing function
	void* lst[]={writer_manu_overwr, writer_manu_append, writer_manu_term, writer_manu_term,
				 writer_auto_overwr, writer_auto_append, writer_auto_term, writer_auto_term};
    writer=lst[(((automatic << 1)+term) << 1)+ append ];

	}
	
    
    while (!feof(stdin)){
    	bytes_in_buffer = fread  (buffer, sizeof(byte), BUFFER_SIZE, stdin);
		
		if (bytes_in_buffer != fwrite (buffer, sizeof(byte), bytes_in_buffer, stdout)){
	    	return(-1);
		}
	
		count += bytes_in_buffer;
        writer();
    
    } //while (!feof(stdin))
    
    return (0);
}
