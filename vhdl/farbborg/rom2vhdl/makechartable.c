#include <stdio.h>
#include <stdint.h>

void bitprint(uint8_t b){
	uint8_t y;
		for(y=0;y<8;y++){
			if(b & 0x80){
				printf("*");
			}else{
				printf(" ");
			}
			b<<=1;
		}
		printf("\n");
}


int main(){
	uint8_t buf[4096];
	uint16_t x, y, z;
	uint8_t tmp;
	fread(buf, 1, 4096, stdin);

	printf("#include <stdint.h>\n\n");
	printf("uint8_t chartable[] = {\n");
	
	for(x=0;x<512;x++){
		printf("\t");
		for(y=0;y<8;y++){
			tmp = 0;
			for(z=0;z<8;z++){
				tmp >>= 1;
				tmp |= (buf[8*x + z] & 0x80)?0x80:0;
				buf[8*x + z] <<= 1;
			}
			printf("0x%02x, ", tmp);
			//bitprint(tmp);
		}
		printf("\n");
	}
	printf("};\n\n");

}

