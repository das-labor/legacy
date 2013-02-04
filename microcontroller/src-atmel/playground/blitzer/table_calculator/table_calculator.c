
#include <stdio.h>
#include <math.h>

int main(){
	//Ladekurve für kontante Leistung.
	//bei konstanter Leistung steigt die Spannung am Blitzelko
	//mit einer Wurzelfunktion
	//U = k * sqrt(t)
	//
	//duty cycle eines boost converters:
	//D = 1 - Uin/Uout
	//
	
	int x;
	uint8_t table[256];
	for(x=0;x<256;x++){
		double a = x;
		double Uout = sqrt(a) * 350.0 / 16.0;
		double Uin = 20.0;
		double D = 1.0 - (Uin/Uout);
		int d = (int) (D * 256.0);
		printf("%d: Uout=%f,  D=%f, d=%d\n", x, Uout, D, d);
		table[x] = d;
	}
	
	printf("\n\n");
	
	for(x=0;x<256;x++){
		printf("%d,", table[x]);
		if((x%16) == 15){
			printf("\n");
		}else{
			printf(" ");
		}
	}
}
