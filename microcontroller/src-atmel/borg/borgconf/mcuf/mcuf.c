
#include "uart.h"
#include "pixel.h"

#define my_uart_getc uart_getc

typedef struct
{
  uint32_t magic;
  uint8_t height_h;
  uint8_t height_l;
  uint8_t width_h;
  uint8_t width_l;
  uint16_t channels;
  uint16_t maxval;
  uint8_t pixels[256];
} mcuf_t;


static void parse_mcuf(mcuf_t * mcuf){
	if(mcuf->height_l == 8 && mcuf->width_l == 18){
		uint8_t x,y;
		for(y=0;y<8;y++){
			for(x=0;x<16;x++){
				setpixel((pixel){15-x, y+4}, mcuf->pixels[y*18 + x + 1] >> 6  );
			}
		}
	}else if(mcuf->height_l == 16 && mcuf->width_l == 16){
		uint8_t x,y;
		for(y=0;y<16;y++){
			for(x=0;x<16;x++){
				setpixel((pixel){15-x, y}, mcuf->pixels[y*16 + x] >> 6  );
			}
		}
	}

}

static uint8_t mcuf_magic[4] = {0x23, 0x54, 0x26, 0x66};


void mcuf_serial_mode(){
	mcuf_t mcuf_packet;
	uint8_t c;

	while(1){
		uint16_t count = 0;
		uint8_t x;
		uint16_t num_pixels;
		do{
			c = my_uart_getc();
			((uint8_t*)&mcuf_packet)[count] = c;
			if(c == mcuf_magic[count] ){
				count ++;
			}else{
				count = 0;
			}
		}while(count != 4);

		for(x=0; x<4; x++){
			c = my_uart_getc();
			((uint8_t*)&mcuf_packet)[count++] = c;
		}
		num_pixels = (uint16_t)mcuf_packet.width_l * mcuf_packet.height_l;
		if(num_pixels <= 256){
		    num_pixels += 4;
			while(num_pixels--){
				c = my_uart_getc();
				((uint8_t*)&mcuf_packet)[count++] = c;
			}
			parse_mcuf(&mcuf_packet);
		}
	}
}


