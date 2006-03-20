
#include "ppi.h"


oktobus::oktobus(){
	stepcount[0] = 0;
	stepcount[1] = 0;
}

int oktobus::init(QString pardev){
	fd = ppi_open(pardev.toAscii());

	ppi_setall(fd, PPIDATA, 0);
}

uint8_t oktobus::readInputs(){
	uint8_t tmp;
	
	ppi_setall(fd, 15, PPICTRL);
	tmp = ppi_getall(fd, PPISTATUS);
	tmp >>=4;
	
	ppi_setall(fd, 15, PPICTRL);
	tmp |= ppi_getall(fd, PPISTATUS) & 0xf0;
	
	tmp ^= 0x88;
	
	return tmp;
}

const static uint8_t motor_sequence[4] = {0, 1, 3, 2};

void oktobus::motorStep(uint8_t motor, bool dir){
	if(dir){
		stepcount[motor] += 1;
	}else{
		stepcount[motor] -= 1;
	}
	ppi_setmask(fd, PPIDATA, 0x03 << (2*motor), motor_sequence[stepcount[motor]%4] << (2*motor) );
}

void oktobus::setOutput(uint8_t output, bool value){
	if(value){
		ppi_set(fd, PPIDATA, output);
	}else{
		ppi_clr(fd, PPIDATA, output);
	}
}
