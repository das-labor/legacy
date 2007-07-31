
#include <stdio.h>

int main(){
	int frame_rate = 200;
	int components = 3;
	int levels = 64;
	int cpu_freq = 16000000;
	
	
	double frequency = levels * levels * components * frame_rate;
	
	double cycle_time = 1.0/frequency;	

	//printf("%.20f\n", cycle_time);

	int x, y=0;
	
	for(x=1;x<64;x++){
		double d;
		unsigned int i;
		d = (double)x * x * cpu_freq * cycle_time;
		d += 0.5;
		i = d;
		printf("\t%d,",i);	
	
		if(++y%8 == 0) printf("\n");
	}
	
	printf("\n");
	
	return 0;
}
