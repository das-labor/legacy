
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char * argv[]){
	uint8_t width;
	if(argc < 3){
			fprintf(stderr,"usage: upload file.bin serial_device\n");
		   	return(-1);
	}

	FILE * fp, * sr;

	fp = fopen(argv[1], "r");
	if(!fp){
			perror("cant open file");
		   	return (-1);
	}

	sr = fopen(argv[2], "w");
	if(!sr){
			perror("cant write to serial port");
			return (-1);
	}

	fseek(fp, 0, SEEK_END);
	uint32_t size,tmp;
	size= ftell(fp);
	
	fseek(fp, 0, SEEK_SET);

	fputc('u' , sr);
	fputc(0, sr);
	fputc(0, sr);
	fputc(size & 0xff, sr);
	fputc(size>>8, sr);
	
	while(1){
		unsigned int data;
		data = fgetc(fp);
		if(feof(fp)) break;
		fputc(data, sr);
	}
	
	if(argc == 4){
		fputc('j', sr);
		fputc(0, sr);
		fputc(0, sr);
	}

	fclose(fp);
	fclose(sr);
		
	return(0);
}

