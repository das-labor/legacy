#include "soc-hw.h"

void main()
{
	gpio0->oe = 0x000000ff;
	for(;;) {
		int i;

		for(i=0; i<8; i++) {
			uint32_t out1, out2;

			out1 = 0x01 << i;
			out2 = 0x80 >> i;
			gpio0->out = out1 | out2;

			msleep(100);
		}
	}
}

